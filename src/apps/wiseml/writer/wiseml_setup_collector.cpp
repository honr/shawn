/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_setup_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlSetupCollector::WisemlSetupCollector(shawn::SimulationController &sc) 
      :  WisemlDataCollector(sc, "wiseml_setup"),
         origin_position_(0.0,0.0,0.0),
         origin_phi_(0.0),
         origin_theta_(0.0)
   {
      
   }
   // ----------------------------------------------------------------------
   WisemlSetupCollector::~WisemlSetupCollector(){}
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   std::string WisemlSetupCollector::name()
      const throw()
   {
      return id_;
   }
   // ----------------------------------------------------------------------
   std::string WisemlSetupCollector::description()
      const throw()
   {
      return "Setup section data";
   }
   // ----------------------------------------------------------------------
   // ----------------------------------------------------------------------
   double WisemlSetupCollector::next_timestamp_after(double time)
   {
      return -1.0;
   }
   // ----------------------------------------------------------------------
   std::string WisemlSetupCollector::generate_xml()
      const
   {
      std::stringstream wml;

      wml << "\t<setup>" << std::endl;
      //Origin
      wml << "\t\t<origin>" << std::endl;
      wml << "\t\t\t<x>" << origin_position_.x() << "</x>" << std::endl;
      wml << "\t\t\t<y>" << origin_position_.y() << "</y>" << std::endl;
      wml << "\t\t\t<z>" << origin_position_.z() << "</z>" << std::endl;
      wml << "\t\t\t<phi>" << origin_phi_ << "</phi>" << std::endl;
      wml << "\t\t\t<theta>" << origin_theta_ << "</theta>" << std::endl;
      wml << "\t\t</origin>" << std::endl;
      //Timeinfo
      wml << "\t\t<timeinfo>" << std::endl;
      wml << "\t\t\t<start>" << start_ << "</start>" << std::endl;
      wml << "\t\t\t<end>" << end_ << "</end>" << std::endl;
      wml << "\t\t</timeinfo>" << std::endl;
      if(interpolation_.empty())
      {
         wml << "\t\t<interpolation/>" <<  std::endl;
      }
      else
      {
         wml << "\t\t<interpolation>" << interpolation_ << 
            "</interpolation>" << std::endl;
      }
      wml << "\t\t<description>" << desc_ << 
         "</description>" << std::endl;
      //Defaults
      wml << "\t\t<defaults>" << std::endl;
      wml << "\t\t\t<nodes>" << std::endl;
      wml << "\t\t\t\t<position>" << std::endl;
      wml << "\t\t\t\t\t<x>" << node_defaults_.posx << "</x>" << std::endl;
      wml << "\t\t\t\t\t<y>" << node_defaults_.posx << "</y>" << std::endl;
      wml << "\t\t\t\t\t<z>" << node_defaults_.posx << "</z>" << std::endl;
      wml << "\t\t\t\t</position>" << std::endl;
      wml << "\t\t\t\t<gateway>";
      if(node_defaults_.gateway)
         wml << "true</gateway>" << std::endl;
      else
         wml << "false</gateway>" << std::endl;
      wml << "\t\t\t\t<image>" << node_defaults_.image << "</image>" << 
         std::endl;
      wml << "\t\t\t\t<nodetype>" << node_defaults_.nodetype << 
         "</nodetype>" << std::endl;
      wml << "\t\t\t\t<description>" << node_defaults_.description << 
         "</description>" << std::endl;
      if(node_defaults_.capabilities.size() > 0)
      {
         for(CapList::const_iterator cit = 
            node_defaults_.capabilities.begin();
            cit != node_defaults_.capabilities.end();
            ++cit)
         {
            wml << "\t\t\t\t<capability>" << std::endl;
            wml << "\t\t\t\t\t<name>" << cit->name << "</name>" << 
               std::endl;
            wml << "\t\t\t\t\t<datatype>" << cit->datatype << 
               "</datatype>" << std::endl;
            wml << "\t\t\t\t\t<unit>" << cit->unit << "</unit>" << 
               std::endl;
            wml << "\t\t\t\t\t<default>" << cit->def_value << 
               "</default>" << std::endl;
            wml << "\t\t\t\t</capability>" << std::endl;
         }
      }
      wml << "\t\t\t</nodes>" << std::endl;
      wml << "\t\t\t<links>" << std::endl;
      wml << "\t\t\t\t<encrypted>";
         if(link_defaults_.is_encrypted)
         {
            wml << "true" << "</encrypted>" << std::endl;
         }
         else
         {
            wml << "false" << "</encrypted>" << std::endl;
         }
         wml << "\t\t\t\t<virtual>";
         if(link_defaults_.is_virtual)
         {
            wml << "true" << "</virtual>" << std::endl;
         }
         else
         {
            wml << "false" << "</virtual>" << std::endl;
         }
      if(link_defaults_.capabilities.size() > 0)
      {
         for(CapList::const_iterator cit = 
            link_defaults_.capabilities.begin();
            cit != link_defaults_.capabilities.end();
            ++cit)
         {
            wml << "\t\t\t\t\t<capability>" << std::endl;
            wml << "\t\t\t\t\t\t<name>" << cit->name << "</name>" << 
               std::endl;
            wml << "\t\t\t\t\t\t<datatype>" << cit->datatype << 
               "</datatype>" << std::endl;
            wml << "\t\t\t\t\t\t<unit>" << cit->unit << "</unit>" << 
               std::endl;
            wml << "\t\t\t\t\t\t<default>" << cit->def_value << 
               "</default>" << std::endl;
            wml << "\t\t\t\t\t</capability>" << std::endl;
         }
      }
      wml << "\t\t\t</links>" << std::endl;
      wml << "\t\t</defaults>" << std::endl;

      //Nodes
      for(std::list<NodeTemplate>::const_iterator nit = nodes_.begin();
         nit != nodes_.end(); ++nit)
      {
         wml << "\t\t<node id=\"" << nit->label << "\">" << std::endl;
         if(nit->posx != node_defaults_.posx || nit->posy != node_defaults_.posy
            || nit->posz != node_defaults_.posz)
         {
            wml << "\t\t\t<position>" << std::endl;
            wml << "\t\t\t\t<x>" << nit->posx << "</x>" << std::endl;
            wml << "\t\t\t\t<y>" << nit->posy << "</y>" << std::endl;
            wml << "\t\t\t\t<z>" << nit->posz << "</z>" << std::endl;
            wml << "\t\t\t</position>" << std::endl;
         }
         if(nit->gateway != node_defaults_.gateway)
         {
            wml << "\t\t\t<gateway>";
            if(nit->gateway)
            {
               wml << "true" << "</gateway>" << std::endl;
            }
            else
            {
               wml << "false" << "</gateway>" << std::endl;
            }
         }
         if(nit->image!= node_defaults_.image 
            && !nit->image.empty())
         {
            wml << "\t\t\t<image>" << nit->image << "</image>" << std::endl;
         }
         if(nit->nodetype != node_defaults_.nodetype 
            && !nit->nodetype.empty())
         {
            wml << "\t\t\t<nodetype>" << nit->nodetype << "</nodetype>"
               << std::endl;
         }
         if(nit->description != node_defaults_.description
            && !nit->description.empty())
         {
            wml << "\t\t\t<description>" << nit->description << 
               "</description>" << std::endl;
         }
         for(CapList::const_iterator cit = nit->capabilities.begin();
            cit != nit->capabilities.end(); ++cit)
         {
            wml << "\t\t\t\t<capability>" << std::endl;
            wml << "\t\t\t\t\t<name>" << cit->name << "</name>" << 
               std::endl;
            wml << "\t\t\t\t\t<datatype>" << cit->datatype << 
               "</datatype>" << std::endl;
            wml << "\t\t\t\t\t<unit>" << cit->unit << "</unit>" << 
               std::endl;
            wml << "\t\t\t\t\t<default>" << cit->def_value << 
               "</default>" << std::endl;
            wml << "\t\t\t\t</capability>" << std::endl;
         }
         wml << "\t\t</node>" << std::endl;
      }


      for(std::list<LinkInfo>::const_iterator lit = links_.begin();
         lit != links_.end(); ++lit)
      {
         wml << "\t\t<link source=\"" << lit->source << "\" target=\"" <<
            lit->target << "\">" << std::endl;
         if(lit->is_encrypted != link_defaults_.is_encrypted)
         {
            wml << "\t\t\t<encrypted>";
            if(lit->is_encrypted)
            {
               wml << "true" << "</encrypted>" << std::endl;
            }
            else
            {
               wml << "false" << "</encrypted>" << std::endl;
            }
         }
         if(lit->is_virtual != link_defaults_.is_virtual)
         {
            wml << "\t\t\t<virtual>";
            if(lit->is_virtual)
            {
               wml << "true" << "</virtual>" << std::endl;
            }
            else
            {
               wml << "false" << "</virtual>" << std::endl;
            }
         }
         for(CapList::const_iterator cit = lit->capabilities.begin();
            cit != lit->capabilities.end(); ++cit)
         {
            wml << "\t\t\t\t<capability>" << std::endl;
            wml << "\t\t\t\t\t<name>" << cit->name << "</name>" << 
               std::endl;
            wml << "\t\t\t\t\t<datatype>" << cit->datatype << 
               "</datatype>" << std::endl;
            wml << "\t\t\t\t\t<unit>" << cit->unit << "</unit>" << 
               std::endl;
            wml << "\t\t\t\t\t<default>" << cit->def_value << 
               "</default>" << std::endl;
            wml << "\t\t\t\t</capability>" << std::endl;
         }
         wml << "\t\t</link>" << std::endl;
      }

      wml << "\t</setup>" << std::endl;

      return wml.str();
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::
      set_origin(shawn::Vec pos, double phi, double theta)
   {
      origin_position_ += pos;
      origin_phi_ = phi;
      origin_theta_ = theta;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::
      set_timeinfo_start(std::string start)
   {
      start_ = start;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::
      set_timeinfo_end(std::string end)
   {
      end_ = end;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::
      set_interpolation(std::string mode)
   {
      interpolation_ = mode;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::
      set_description(std::string desc)
   {
      desc_ = desc;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::set_node_defaults(NodeTemplate &defaults)
   {
      node_defaults_.capabilities = defaults.capabilities;
      node_defaults_.description = defaults.description;
      node_defaults_.gateway = defaults.gateway;
      node_defaults_.image = defaults.image;
      node_defaults_.nodetype = defaults.nodetype;
      node_defaults_.posx = defaults.posx;
      node_defaults_.posy = defaults.posy;
      node_defaults_.posz = defaults.posz;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::set_default_node_capability(Capability &cap)
   {
      node_defaults_.capabilities.push_back(cap);
   }
      // ----------------------------------------------------------------------
   void WisemlSetupCollector::set_link_defaults(LinkInfo &defaults)
   {
      link_defaults_.capabilities = defaults.capabilities;
      link_defaults_.is_encrypted = defaults.is_encrypted;
      link_defaults_.is_virtual = defaults.is_virtual;
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::set_default_link_capability(Capability &cap)
   {
      link_defaults_.capabilities.push_back(cap);
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::gather_topology()
   {
      shawn::World::const_node_iterator nodes = sc_.world().begin_nodes();
      for(nodes; nodes != sc_.world().end_nodes(); ++nodes)
      {
         NodeTemplate ntempl(node_defaults_);
         ntempl.capabilities.clear();
         ntempl.label = nodes->label();
         shawn::Vec pos = nodes->real_position();
         ntempl.posx = pos.x();
         ntempl.posy = pos.y();
         ntempl.posz = pos.z();
         nodes_.push_back(ntempl);
         
         shawn::World::const_adjacency_iterator adjacency = 
            sc_.world().begin_adjacent_nodes(*nodes,shawn::EdgeModel::CD_OUT);
         for(adjacency; adjacency != sc_.world().end_adjacent_nodes(*nodes);
            ++adjacency)
         {
            LinkInfo ltempl;
            ltempl.is_encrypted = link_defaults_.is_encrypted;
            ltempl.is_virtual = link_defaults_.is_virtual;
            ltempl.source = nodes->label();
            ltempl.target = adjacency->label();
            links_.push_back(ltempl);
         }
      }
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::add_node(NodeTemplate &node)
   {
      nodes_.push_back(node);
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::add_link(LinkInfo &link)
   {
      links_.push_back(link);
   }
   // ----------------------------------------------------------------------
   void WisemlSetupCollector::clear()
   {

   }


}
#endif
