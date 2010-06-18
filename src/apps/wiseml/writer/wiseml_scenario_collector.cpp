/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2010 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/
#include "apps/wiseml/writer/wiseml_scenario_collector.h"
#ifdef ENABLE_WISEML
namespace wiseml
{
   WisemlScenarioCollector::
      WisemlScenarioCollector(shawn::SimulationController &sc,
      std::string id) 
      :  WisemlDataCollector(sc, id)
   {}
   // ----------------------------------------------------------------------
   WisemlScenarioCollector::~WisemlScenarioCollector(){}
   // ----------------------------------------------------------------------
   std::string WisemlScenarioCollector::name()
      const throw()
   {
      return id_;
   }
   // ----------------------------------------------------------------------
   std::string WisemlScenarioCollector::description()
      const throw()
   {
      return "Scenario section data";
   }
   // ----------------------------------------------------------------------
   double WisemlScenarioCollector::next_timestamp_after(double time)
   {
      double next = -1.0;
      if(items_.upper_bound(time) != items_.end())
         next = items_.upper_bound(time)->first;
      return next;
   }
   // ----------------------------------------------------------------------
   // Data collection methods
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::enable_node(std::string node)
   {
      double timestamp = current_time();
      string item = "en,";
      item += node;
      items_.insert(pair<double, string>(timestamp, item));
   }
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::disable_node(std::string node)
   {
      double timestamp = current_time();
      string item = "dn,";
      item += node;
      items_.insert(pair<double, string>(timestamp, item));
   }
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::
      enable_link(std::string src, std::string dst)
   {
      double timestamp = current_time();
      string item = "el,";
      item += src + string(",") + dst;
      items_.insert(pair<double, string>(timestamp, item));
   }
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::
      disable_link(std::string src, std::string dst)
   {
      double timestamp = current_time();
      string item = "dl,";
      item += src + string(",") + dst;
      items_.insert(pair<double, string>(timestamp, item));
   }
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::node_movement(std::string node)
   {
      double timestamp = current_time();
      shawn::Vec pos = 
         sc_.world().find_node_by_label(node)->real_position();
      std::stringstream item;
      item << "np," << node << "," << pos.x() << 
         "," << pos.y() << "," << pos.z();
      items_.insert(pair<double, string>(timestamp, item.str()));
   }
   // ----------------------------------------------------------------------
   void WisemlScenarioCollector::capability_value(std::string node, 
         std::string capability, std::string value)
   {
      double timestamp = current_time();
      std::stringstream item;
      item << "nc," << node << "," << capability << "," << value;
      items_.insert(pair<double, string>(timestamp, item.str()));
   }
   // ----------------------------------------------------------------------
   std::string WisemlScenarioCollector::generate_xml()
      const
   {
      std::stringstream wml;
      wml << "\t" << "<scenario id=\"" << id_ << "\">" << std::endl;
      double next_timestamp = items_.lower_bound(0.0)->first;
      while(next_timestamp >= 0.0)
      {
         wml << "\t\t" << "<timestamp>" << next_timestamp << 
            "</timestamp>" << std::endl;

         wml << generate_timestamp_xml(next_timestamp);

         if(items_.upper_bound(next_timestamp)!= items_.end())
            next_timestamp = items_.upper_bound(next_timestamp)->first;
         else
            next_timestamp = -1.0;
      }
      wml << "\t</scenario>" << std::endl;
      return wml.str();
   }
   // ----------------------------------------------------------------------
      // ----------------------------------------------------------------------
   std::string 
      WisemlScenarioCollector::generate_timestamp_xml(double timestamp) 
      const
   {
      std::stringstream wml;
      std::map<string, NodeTemplate*> nodes;
      std::multimap<double, std::string>::const_iterator ts_begin = 
         items_.lower_bound(timestamp);
      std::multimap<double, std::string>::const_iterator ts_end = 
         items_.upper_bound(timestamp);

      for(std::multimap<double, std::string>::const_iterator it = ts_begin;
         it != ts_end; ++it)
      {
         std::list<std::string> data(make_list(it->second));
         list<string>::iterator dit = data.begin();
         std::string datatype = dit->c_str();
         
         if(datatype == "en")
         {
            ++dit;
            std::string label = dit->c_str();
            wml << "\t\t<enablenode id=\"" << label << "\"/>" << std::endl;
         }
         else if(datatype == "dn")
         {
            ++dit;
            std::string label = dit->c_str();
            wml << "\t\t<disablenode id=\"" << label << "\"/>" << std::endl;
         }
         else if(datatype == "el")
         {
            ++dit; std::string source = dit->c_str();
            ++dit; std::string target = dit->c_str();
            wml << "\t\t<enablelink source=\"" << source << "\"/>" << 
               "target=\"" << target << "\"/>" << std::endl;
         }
         else if(datatype == "dl")
         {
            ++dit; std::string source = dit->c_str();
            ++dit; std::string target = dit->c_str();
            wml << "\t\t<disablelink source=\"" << source << "\"/>" << 
               "target=\"" << target << "\"/>" << std::endl;
         }
         else if(datatype == "np")
         {
            ++dit;
            std::string label = dit->c_str();

            NodeTemplate *tmp;
            if(nodes.find(label) != nodes.end())
            {
               tmp = nodes.find(label)->second;
            }
            else
            {
               tmp = new NodeTemplate();
               nodes.insert(pair<string, NodeTemplate*>(label, tmp));
            }
            tmp->label = label;
            ++dit;
            tmp->posx = atof(dit->c_str());
            ++dit;
            tmp->posy= atof(dit->c_str());
            ++dit;
            tmp->posz = atof(dit->c_str());
            Capability dummy;
            dummy.name = "positioned";
            tmp->capabilities.push_front(dummy);
         }
         else if(datatype == "nc")
         {
            ++dit;
            std::string label = dit->c_str();

            NodeTemplate *tmp;
            if(nodes.find(label) != nodes.end())
            {
               tmp = nodes.find(label)->second;
            }
            else
            {
               tmp = new NodeTemplate();
               nodes.insert(pair<string, NodeTemplate*>(label, tmp));
            }
            tmp->label = label;
            Capability cap;
            ++dit;
            cap.name = dit->c_str();
            ++dit;
            cap.def_value = dit->c_str();
            tmp->capabilities.push_back(cap);
         }

         
      }

      for(map<string, NodeTemplate*>::iterator it = nodes.begin();
         it != nodes.end(); ++it)
      {
         wml << "\t\t<node id=\"" << it->second->label << "\">" << std::endl;
         if(it->second->capabilities.front().name == "positioned")
         {
            wml << "\t\t\t<position>" << std::endl;
            wml << "\t\t\t\t<x>" << it->second->posx << "</x>" << std::endl;
            wml << "\t\t\t\t<y>" << it->second->posy << "</y>" << std::endl;
            wml << "\t\t\t\t<z>" << it->second->posz << "</z>" << std::endl;
            wml << "\t\t\t</position>" << std::endl;
         }

         for(CapList::iterator cit = it->second->capabilities.begin();
            cit != it->second->capabilities.end(); ++cit)
         {
            if(cit->name != "positioned")
               wml << "\t\t\t<data key=\"" << cit->name << "\">" << 
                  cit->def_value << "</data>" << std::endl;
         }
         wml << "\t\t</node>" << std::endl;
      }
      
      return wml.str();
   }
}
#endif
