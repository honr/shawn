/************************************************************************
 ** Source  $HeadURL: https://www.ldv.ei.tum.de/repos/vertrauenstuds/shawn-apps/trunk/sumo/sumo_net.h $
 ** Version $Revision: 416 $
 ** Id      $Id: sumo_net.h 416 2010-09-01 22:12:09Z wbam $
 ************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2010 Josef Schlittenlacher                           **
 ** Copyright (C) 2010 Technische Unversitaet Muenchen (www.tum.de)    **
 ** All rights reserved.                                               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SUMO_SUMO_NET_H
#define __SHAWN_APPS_SUMO_SUMO_NET_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

#include "sys/vec.h"
#include "sys/xml/sax_reader.h"
#include <vector>
#include <map>
#include <string>

namespace shawn
{

class SumoNet
{
public:

   /**
   * @struct SumoPosition
   *
   * @brief Position expressed with SUMO parameters (edge_id, lane_id, lane_position)
   */
   struct SumoPosition
   {
      std::string edge_id;
      std::string lane_id;
      double lane_position;
   };

   SumoNet( const std::string& path_net_file );

   virtual ~SumoNet();

   /**
   * Compute the x and y coordinates out of a SUMO position (lane, lane position)
   */
   Vec sumo_pos_to_vec( const SumoPosition& sumo_position ) const;

private:
   /** \brief This helper class parses the SUMO net file with the SAX parser.
   	*
   	*  At present, it can only parse the lane tags and extract their ids and shapes.
   	*/
   class SUMONetReader : public shawn::xml::SAXSimpleSkipReader
   {
   public:
      /** initializes the SAX parser.
       *
       * \param filename the file name or URI of the SUMO net file.
       */
      SUMONetReader( const std::string& file_name ) :
            SAXSimpleSkipReader()
      {
         SAXSimpleSkipReader::set_document_uri( file_name );
      }

      /** \brief Extracts the ids and shapes of the lanes in the net file.
       *
       *  This method parses the net file for lane tags. For each lane tag,
       *  it collects the id and shape in the resulting map. This map must be
       *  pre-allocated by the user.
       *
       *  \param lanesPtr A pointer to the map where the lanes should be stored in
       *  \return The same pointer as given by lanesPtr
       */
      std::map< std::string, std::vector< Vec > >*
      extract_lanes( std::map < std::string, std::vector< Vec > > * lanes_ptr ) throw( std::runtime_error );

   protected:
      /** \brief Processes the lane tags.
       *
       *  A callback function of the SAXSimpleSkipReader. This method evaluates
       *  the lane tag and stores it in the map.
       *
       *  \param name the first parameter is the name of the tag found (always "lane")
       *  \param atts a list of the attributes of the lane tag
       */
      virtual void
      skip_target_reached( std::string, shawn::xml::AttList& atts )
      throw( std::runtime_error );

   private:
      /** A pointer to the map all lanes are stored in. This map has been pre-allocated by the user.
       */
      std::map< std::string, std::vector< Vec > >* lanes_ptr_;
   };

   /**
   * List of lane shapes indexed by lane ids.
   *
   * A lane shape is a vector of cartesian positions.
   */
   std::map< std::string, std::vector<Vec> > map_lanes_;

};

}

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_SUMO_NET_H */
