/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_XML_READING_H
#define __SHAWN_APPS_XML_READING_H

#include "../buildfiles/_apps_enable_cmake.h"
#ifdef	ENABLE_READING

#include "sys/simulation/simulation_controller.h"
#include "apps/reading/reading.h"
#include "apps/reading/xml_reading_parser.h"
#include "apps/reading/xml_reading_base.h"
#include "apps/reading/reading_keeper.h"
#include "apps/reading/simple_reading.h"
#include "sys/util/string_conv.h" 
#include "sys/event_scheduler.h"
#include "sys/world.h"
#include "sys/vec.h"


namespace xmlreading
{
	template <typename T>
	class XMLReading : public reading::SimpleReading<T>, public xmlreading::XMLReadingBase, public shawn::EventScheduler::EventHandler
	{
	public:
		typedef T ValueType;
		Readings* xmlreadings_;
		Settings* xmlsettings_;
	private:
		std::string reading_data_type;
		std::string reading_file_;
		double last_event_time_;
		double past_world_time_;
		double next_invalidation_event_time_;
		xmlreading::XMLReadingParser* xmlreadingparser_;
		xmlreading::Readings* xmlparserreadings_;
		shawn::EventScheduler::EventHandle event_handle_;
		shawn::World* world_;
		bool rbv_not_empty;
	public:
		///@name construction/destruction
        	///@{
		XMLReading()
		{
			xmlreadings_ = new Readings();
			xmlsettings_ = new Settings();
			xmlparserreadings_ = new Readings();
			rbv_not_empty = false;
		}
		virtual ~XMLReading()
		{
			xmlreadings_ = NULL;
			xmlsettings_ = NULL;
			xmlparserreadings_ = NULL;
			world_ = NULL;
			xmlreadingparser_ = NULL;
			delete xmlreadings_;
			delete xmlsettings_;
			delete xmlparserreadings_;
			delete world_;
			delete xmlreadingparser_;
		}
		///@}
		// ----------------------------------------------------------------------
		///@name world access
		///@{
		/** Set the world controlled by the simulation controller */
		virtual void set_world( shawn::World& w ) throw()
		{
			world_ = &w;
		}
		///@}
		//-----------------------------------------------------------------------
		/** Prepares the actual parsing process */
		virtual void prepare_parsing(shawn::SimulationController& sc) throw()
		{
			TRACE("XMLREADING: prepare parsing");
			set_world(sc.world_w());
			
			/** Sets the relative path to the XML file which has to be parsed */
			reading_file_ = sc.environment().required_string_param("reading_file");
			
			/** Instantiate ReadingParser with current type of XMLReading<T> */
			xmlreadingparser_ = new XMLReadingParser(*this);
			
			/** Tell the parser where to find the XML file */
			xmlreadingparser_->set_document_uri(&reading_file_ [0]);
			
			/** Parse the given file*/
			xmlreadingparser_->parse();
		}
		//-------------------------------------------------------------
		
		//-------------------------------------------------------------
		/** When the parser delivers new setting data the XMLReading needs to get those information in order to inform his listeners about what to do with the values received out of the xml reading file. Here it is done by copying the information into a local (public) setting data variable.  */
		virtual void receive_and_pass_settings(xmlreading::Settings& settings, std::string& reading_data_type) throw()
		{
			TRACE("XMLREADING: receive and pass settings");
			xmlsettings_ = &settings;
			has_listeners();
			if(rbv_not_empty)
			{
#ifndef WIN32
#warning THIS IS NON FUNCTIONAL, CORRECT THE LINE BELOW HERE
#endif

				/*for(reading::Reading::ReadingChangedHandlerList::iterator it = rbv_workaround->begin(); it != rbv_workaround->end(); ++it)
				{
					it->first->receiving_settings(*xmlsettings_, reading_data_type);
				}*/
			}
			else TRACE("XMLReading: no listeners");
		}
		//-------------------------------------------------------------
		/** When the parser delivers new values they need to be inserted into the local Readings varaible (xmlreadings) and erased out of the Readings variable parserreadings in order to prepare the next event. */ 
		virtual void receiving_new_values(xmlreading::Readings& parserreadings) throw()
		{	
			TRACE("XMLREADING: receiving new values");
			bool insert = false;
			xmlparserreadings_ = &parserreadings;
			refresh_value_durations();
			has_listeners();
			
			for(Readings::iterator rp = xmlparserreadings_->begin(); rp!= xmlparserreadings_->end(); ++rp)
			{
				int i = xmlreadings_->size();
				xmlreadings_->insert(*rp);
				int j = xmlreadings_->size();
								
				if( i == j )
				{
					for(Readings::iterator xml= xmlreadings_->begin(); xml!=xmlreadings_->end(); ++xml)
					{
						if(rp->first == xml->first)
						{
							if(rp->second.first != xml->second.first)
							{
								xmlreadings_->erase(xml);
								insert = true;
								lookup_reading_changed_handlers(rp, insert);
								xmlreadings_->insert(*rp);
							}
							else if(rp->second.second != xml->second.second)
							{
								xmlreadings_->erase(xml);
								xmlreadings_->insert(*rp);
							}
						}	
					}
				}else
				{
					insert = true;
					lookup_reading_changed_handlers(rp, insert);
				}
				xmlparserreadings_->erase(rp);
			}
			TRACE_READINGS(show_xmlreadings());
		}
		///@name event handling
		///@{
		/** Sets the next event in relation to the next parse event of the parser and the next invalidation event of the reading*/
		virtual void set_next_event() throw()
		{		
			TRACE("XMLREADING: set next event");
			set_next_invalidation_event_time();
			if(xmlreadings_->size() == 0 && !xmlreadingparser_->parsing_done_)
			{
				TRACE("Initializing parsing event at: "<<xmlreadingparser_->get_next_parse_event_time());
				
				event_handle_ = world_->scheduler_w().new_event(*xmlreadingparser_, 
				xmlreadingparser_->get_next_parse_event_time(), NULL);
				
			}else if(xmlreadings_->size() != 0 && xmlreadingparser_->parsing_done_)
			{
				TRACE("Initializing invalidation event at: "<<next_invalidation_event_time_);
				event_handle_ = world_->scheduler_w().new_event(*this, next_invalidation_event_time_, NULL);
   				
			}else if(xmlreadings_->size() == 0 && xmlreadingparser_->parsing_done_)
			{
				TRACE("Done parsing & passing values");
								
			}else if(xmlreadings_->size() !=0 && next_invalidation_event_time_ <= xmlreadingparser_->get_next_parse_event_time())
			{	
				TRACE("Initializing invalidation event at: "<<next_invalidation_event_time_);
				event_handle_ = world_->scheduler_w().new_event(*this, next_invalidation_event_time_, NULL);
				
			}else if(xmlreadings_->size() !=0 && next_invalidation_event_time_ > xmlreadingparser_->get_next_parse_event_time())
			{
				TRACE("Initializing parsing event at: "<<xmlreadingparser_->get_next_parse_event_time());
				event_handle_ = world_->scheduler_w().new_event(*xmlreadingparser_, xmlreadingparser_->get_next_parse_event_time(), NULL);
			
			}
		}
		///
		virtual void timeout(shawn::EventScheduler& event_scheduler, shawn::EventScheduler::EventHandle event_handle, double time, shawn::EventScheduler::EventTagHandle& event_tag_handle) throw()
		{
			TRACE("XMLREADING: invalidation event at: "<<next_invalidation_event_time_);
			current_time_ = next_invalidation_event_time_;
			notify_reading_changed_handlers();
			set_next_event();
		}
		///@}
		///@name class own methods
		///@{
		/** Shows the actual content of the Readings variable xmlreadings_.*/
		virtual void show_xmlreadings() throw()
		{
			std::cout<<"XMLREADING: current xmlreadings"<<std::endl;
			for(Readings::iterator it = xmlreadings_->begin(); it!= xmlreadings_->end(); ++it)
			{
				std::cout<<"	"<<it->first.x()<<" "<<it->first.y()<<" "<<it->first.z()<<"   "<<it->second.first<<" "<<it->second.second<<std::endl;
			}
		}
		///@}
	private:
		///@name Readings related methods
		///@{
		/** If the duration of a value reaches 0 the appropriate reading listeners, as there are e.g. sensors, 
		  * have to be informed about changes in their area ( domain ).*/    
		virtual void notify_reading_changed_handlers() throw()
		{
			TRACE("XMLREADING: notify reading changed handlers");
			refresh_value_durations();
			TRACE_READINGS(show_xmlreadings());
			for(Readings::iterator xml = xmlreadings_->begin(); xml != xmlreadings_->end(); ++xml)
			{
				if(xml->second.second <= 0)
				{
					bool insert = false;
					lookup_reading_changed_handlers(xml, insert);	
					xmlreadings_->erase(xml);
					if(!xmlreadings_->size() == 0)
						--xml;
				}
			
			}	
		}
		//-----------------------------------------------------------------------
		/** While simulating, time goes on. All value durations have to be updated with reference to 
		  * the actual simulation time.*/ 
		virtual void refresh_value_durations() throw()
		{	
			TRACE("XMLREADING: refresh value durations");
			past_world_time_ = current_time_ - last_event_time_;
			for(Readings::iterator it = xmlreadings_->begin(); it!=xmlreadings_->end(); ++it)
			{
				it->second.second = it->second.second - past_world_time_;
				if(it->second.second < EPSILON)
					it->second.second = 0.0;
			}
			last_event_time_ = current_time_;
		}
		//-------------------------------------------------------------
		/** Sets the next invalidation event time. The invalidation event time is addicted to the values durations. */
		virtual void set_next_invalidation_event_time() throw()
		{
			TRACE("XMLREADING: set next invalidation event time");
			next_invalidation_event_time_ = INT_MAX;
			if(xmlreadings_ != NULL)
			{
				for(Readings::iterator it = xmlreadings_->begin(); it!=xmlreadings_->end(); ++it)
				{
					if(it->second.second + current_time_ <= next_invalidation_event_time_)
					{
						next_invalidation_event_time_ = it->second.second + current_time_;
						
					}	
				}
			}
		}
		//-------------------------------------------------------------
		/** Returns the next invalidation event time. */
		virtual double get_next_invalidation_event_time() throw() { return next_invalidation_event_time_;}
		//---------------------------type--------------------------------------------
		/** Checks, if there are any listeners (reading changed handlers)*/
		virtual void lookup_reading_changed_handlers(Readings::iterator iter, bool insert) throw()
		{
			TRACE("XMLREADING: lookup reading changed handlers");
			if(rbv_not_empty)
			{
#ifndef WIN32
#warning THIS IS NON FUNCTIONAL, CORRECT THE LINE BELOW HERE
#endif
/*
				for(ReadingChangedHandlerList::iterator it = rbv.begin(); it != rbv.end(); ++it)
				{	
					if( it->second.contains(iter->first) )
					{
						if(insert)
						{
							it->first->insert_domain_value(iter->first, iter->second.first);
						}else
						{
							it->first->erase_domain_value(iter->first);
						}
						it->first->reading_changed(*this, it->second);
					}
				}
*/
			} else {
				TRACE("No Listeners available");
			}
		}
		///
		/** checks if the sensor has listeners*/
		virtual void has_listeners() throw()
		{
#ifndef WIN32
#warning THIS IS NON FUNCTIONAL, CORRECT THE LINE BELOW HERE
#endif
/*
			if(!rbv.empty())
				rbv_not_empty = true;
			else 
				rbv_not_empty = false;
*/
		}
		///@}
	};
	
	
	
	
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	class XMLIntegerReading : public XMLReading<int>
	{
	public:
		///@name construction / destruction
		///@{
		XMLIntegerReading(std::string& reading_name_) : reading_name (reading_name_){}
		///
		~XMLIntegerReading(){}
		///@}
		///@name keeper managed
		///@{
		std::string name() const throw() {return reading_name;}
		///
		std::string description() const throw() {return "Reading which holds integer values for the simulation";}
		///@}
		///@name NodeSensor method
		///@{
		ValueType value( const shawn::Vec& v ) const throw()
		{
			std::string s = "";
			s = xmlreadings_->find(v)->second.first;
			return shawn::conv_string_to_int(s);
		}
		///@}
	private:
		std::string reading_name;
	};
	DECLARE_HANDLES(XMLIntegerReading);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	class XMLDoubleReading : public XMLReading<double>
	{
	public:
		///@name construction / destruction
		///@{
		XMLDoubleReading(std::string& reading_name_) : reading_name (reading_name_){}
		///
		~XMLDoubleReading(){}
		///@}
		///@name keeper managed
		///@{
		std::string name() const throw() {return reading_name;}
		///
		std::string description() const throw() {return "Reading which holds double values for the simulation";}
		///@}
		///@name NodeSensor method
		///@{
		ValueType XMLDoubleReading::value( const shawn::Vec& v ) const throw()
		
		{
			std::string s = "";
			s = xmlreadings_->find(v)->second.first;
			return shawn::conv_string_to_double(s);
		}
		///@}
	private:
		std::string reading_name;
	};
	DECLARE_HANDLES(XMLDoubleReading);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	class XMLBoolReading : public XMLReading<bool>
	{
	public:
		///@name construction / destruction
		///@{
		XMLBoolReading(std::string& reading_name_) : reading_name (reading_name_){}
		///
		~XMLBoolReading(){}
		///@}
		///@name keeper managed
		///@{
		std::string name() const throw() {return reading_name;}
		///
		std::string description() const throw() {return "Reading which holds bool values for the simulation";}
		///@}
		///@name NodeSensor method
		///@{
		ValueType value( const shawn::Vec& v ) const throw()
		{
			std::string s = "";
			s = xmlreadings_->find(v)->second.first;
			return shawn::conv_string_to_bool(s);
		}
		///@}
	private:
		std::string reading_name;
	};
	DECLARE_HANDLES(XMLBoolReading);
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	class XMLStringReading : public XMLReading<std::string>
	{
	public:
		///@name construction / destruction
		///@{
		XMLStringReading(std::string& reading_name_) : reading_name (reading_name_){}
		///
		~XMLStringReading(){}
		///@}
		///@name keeper managed
		///@{
		std::string name() const throw() {return reading_name;}
		///
		std::string description() const throw() {return "Reading which holds string values for the simulation";}
		///@}
		///@name NodeSensor method
		///@{
		ValueType value( const shawn::Vec& v ) const throw()
		{
			std::string s = "";
			s = xmlreadings_->find(v)->second.first;
			return s;
		}
		///@}
	private:
		std::string reading_name;
	};
	DECLARE_HANDLES(XMLStringReading);
}

#endif
#endif

/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/apps/reading/xml_reading.h,v $
 * Version $Revision: 1.8 $
 * Date    $Date: 2006/07/31 11:42:09 $
 *-----------------------------------------------------------------------*/

