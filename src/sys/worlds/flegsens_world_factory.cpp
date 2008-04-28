/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#include "sys/worlds/flegsens_world_factory.h"
#include "sys/world.h"
#include "sys/vec.h"
#include "sys/util/string_conv.h"
#include "sys/comm_models/manual_communication_model.h"
#include "sys/node_movements/no_movement.h"
#include "sys/taggings/tag_factory.h"
#include "sys/taggings/tag_factory_keeper.h"
#include "sys/simulation/simulation_controller.h"

//TODO: Still necessary? Included these files for KiSeq-Loading
#include "sys/util/string_conv.h"
#include "sys/comm_models/rim_comm_model.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h> 

using namespace std;
using namespace shawn::xml;

namespace shawn {

// ----------------------------------------------------------------------
FlegsensWorldFactory::FlegsensWorldFactory() :
	node_count_(0), world_(NULL), parsing_state_(Unknown), current_node_(NULL), flegsens_xml_world() {
}

void FlegsensWorldFactory::set_parameters(int in_x_count, int in_y_count, double in_x_dist, double in_y_dist, double in_x_off,
											int in_gps_row1, int in_gps_row1_interval, int in_gps_row1_offset,
											int in_gps_row2, int in_gps_row2_interval, int in_gps_row2_offset) {
	x_count_ = in_x_count;	// Anzahl Sensorknoten auf der x-Achse
	y_count_ = in_y_count;	// Anzahl Sensorknoten auf der y-Achse
	x_dist_ = in_x_dist;  	// Abstand zwischen zwei Knoten auf der x-Achse in [m]
	y_dist_ = in_y_dist;  	// Abstand zwischen zwei Knoten auf der y-Achse in [m] 
	x_off_ = in_x_off;   	// Offset zwischen zwei Reihen auf der x-Achse in [m]
	
	gps_row1_ = in_gps_row1;
	gps_row1_interval_ = in_gps_row1_interval;
	gps_row1_offset_ = in_gps_row1_offset;
	gps_row2_ = in_gps_row2;
	gps_row2_interval_ = in_gps_row2_interval;
	gps_row2_offset_ = in_gps_row2_offset;
	
}


// ----------------------------------------------------------------------
void FlegsensWorldFactory::fill_world(shawn::World& w) throw() {
	world_ = &w;
	sim_controller_ = &w.simulation_controller_w();
	set_tag_factory_keeper(w.simulation_controller_w().tag_factory_keeper_w() );

	// {Filling the FlegSens World}
	char text_buf[100];
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
	bool is_anchor = false;
	
	flegsens_xml_world.appendLine("<scenario>");
	flegsens_xml_world.appendLine("<snapshot id=\"0\">");
	
	// Create x and y position of all nodes
	for (int j=0; j<y_count_; j++) {
		for (int i=0; i<x_count_; i++) {
			x = (i*x_dist_) + (x_off_*(j%2));
			y = (j*y_dist_);
			z = 0.0;
			
			
			sprintf(text_buf, "<node id=\"node_%d-%d\">", i,j);
			flegsens_xml_world.appendLine(text_buf);
									
			sprintf(text_buf, "<location x=\"%f.2\" y=\"%f.2\" z=\"%f.2\" />", x,y,z);
			flegsens_xml_world.appendLine(text_buf);	
			
			if ((j==gps_row1_)&&(((i+gps_row1_offset_)%gps_row1_interval_)==0)) {
				is_anchor = true;
			} else if ((j==gps_row2_)&&(((i+gps_row2_offset_)%gps_row2_interval_)==0)) {
				is_anchor = true;
			} else {
				is_anchor = false;
			}
			
			if (is_anchor) {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"anchor\" value=\"true\"/>");
			} else {
				flegsens_xml_world.appendLine("<tag type=\"bool\" name=\"anchor\" value=\"false\"/>");
			}
			flegsens_xml_world.appendLine("</node>");
		}
	}
	flegsens_xml_world.appendLine("</snapshot>");
	flegsens_xml_world.appendLine("</scenario>");	
	// END {Filling the FlegSens World}
	
	parse_Object(&flegsens_xml_world);
	sim_controller_ = NULL;
	world_ = NULL;
}

// ----------------------------------------------------------------------
FlegsensWorldFactory::~FlegsensWorldFactory() {
	// empty
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::skip_target_reached(std::string, AttList& atts) {
	cerr << "Load World: Snapshot found: " << attribute("id", atts) << endl;
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::handle_start_element(string name, AttList& atts)
		throw(runtime_error) {
	SAXSkipReader::handle_start_element(name, atts);

	if ( !skipping() )
		parse_xml(name, atts, true);
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::handle_end_element(string name) throw(runtime_error) {
	SAXSkipReader::handle_end_element(name);
	AttList a;

	if ( !skipping() )
		parse_xml(name, a, false);
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::parse_xml(string name, AttList& atts,
		bool opening_tag) throw(runtime_error) {
	if (parsing_state_ == Done)
		return;

	if ("node" == name)
		handle_tag_node(atts, opening_tag);
	else if ("location" == name && opening_tag && parsing_state_ == Node)
		handle_tag_location(atts, opening_tag);
	else if ("snapshot" == name)
		handle_tag_snapshot(atts, opening_tag);
	else if ("world" == name)
		handle_tag_world(atts, opening_tag);
	else if ("sizehint" == name && opening_tag)
		handle_tag_sizehint(atts, opening_tag);
	else if ("scenario" == name)
		handle_tag_scenario(atts, opening_tag);
	else if ("entry" == name && opening_tag)
		handle_tag_entry(atts);
	else if ("tag" == name) {
		TagContainer* tc = world_;
		if (parsing_state_ == Node)
			tc = current_node_;

		if (opening_tag)
			handle_open_tag_tag(atts, *tc);
		else
			handle_close_tag_tag(atts, *tc);
	}
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_scenario(AttList& atts,
		bool opening_tag) {
	opening_tag ? parsing_state_ = Scenario : parsing_state_ = Done;
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_world(AttList& atts,
		bool opening_tag) {
	opening_tag ? parsing_state_ = World : parsing_state_ = Snapshot;
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_snapshot(AttList& atts,
		bool opening_tag) {
	if (opening_tag)
		parsing_state_ = Snapshot;
	else {
		interrupt();
		parsing_state_ = Done; //One snapshot loaded
		cerr << "Load World: Done." << endl;
	}
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_node(AttList& atts,
		bool opening_tag) {
	if (opening_tag) {
		parsing_state_ = Node;
		current_node_ = new_node(Vec(0, 0, 0));//new Node();
		current_node_->set_label(attribute("id", atts));
	} else {
		add_node(current_node_);
		parsing_state_ = Snapshot;
		node_count_++;

		if (node_count_ % 5000 == 0)
			cerr << "  Node count: " << node_count_ << " so far" << endl;
	}
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::add_node(shawn::Node* n) {
	//n=current_node_
	world_->add_node( *n);
}
// ----------------------------------------------------------------------

inline void FlegsensWorldFactory::handle_tag_location(AttList& atts,
		bool opening_tag) {
	if (!opening_tag)
		return;

	double x, y, z;
	string tmp;

	tmp = attribute("x", atts);
	x = tmp != "" ? atof(tmp.c_str()) : 0.0;

	tmp = attribute("y", atts);
	y = tmp != "" ? atof(tmp.c_str()) : 0.0;

	tmp = attribute("z", atts);
	z = tmp != "" ? atof(tmp.c_str()) : 0.0;

	Vec loc(x, y, z);
	current_node_->set_real_position(loc);
}

// ----------------------------------------------------------------------
inline void FlegsensWorldFactory::handle_tag_sizehint(AttList& atts,
		bool opening_tag) {
	if (!opening_tag)
		return;

	world_->set_size_hint(conv_string_to_double(attribute("value", atts) ));
}

// ----------------------------------------------------------------------
void FlegsensWorldFactory::set_snapshot_id(const std::string tim) throw() {
	cerr << "Load World: Loading snapshot: " << tim << endl;
	set_skip_target("snapshot");
	set_skip_target_add_attr("id", tim);
}

// ----------------------------------------------------------------------

void FlegsensWorldFactory::create_xml_file() {
	cerr << "Creating World and Node XML Representation..." << endl;
	
}

// ----------------------------------------------------------------------


}
