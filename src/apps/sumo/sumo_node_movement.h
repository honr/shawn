/************************************************************************
 ** Source  $HeadURL: https://www.ldv.ei.tum.de/repos/vertrauenstuds/shawn-apps/trunk/sumo/sumo_node_movement.h $
 ** Version $Revision: 416 $
 ** Id      $Id: sumo_node_movement.h 416 2010-09-01 22:12:09Z wbam $
 ************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2010 Josef Schlittenlacher                           **
 ** Copyright (C) 2010 Technische Unversitaet Muenchen (www.tum.de)    **
 ** All rights reserved.                                               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H
#define __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H
#include "_apps_enable_cmake.h"
#ifdef ENABLE_SUMO

#include "sumo_net.h"
#include "sys/node_movements/playback/movement_info.h"
#include "sys/node_movements/playback/node_movement_creator.h"
#include "sys/processors/processor_factory.h"
#include "sys/vec.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>

namespace shawn
{
class SimulationController;

/** Moves nodes based on a SUMO dump file.
 *
 * Usage in the Shawn configuration file:
 *
 *   node_movement mode=sumo net_file=<NET> dump_file=<DUMP> processors=<PROCESSOR>
 *
 * mode=sumo:  Activates this class
 * net_file:   The SUMO net file on which the SUMO dump file is based
 * dump_file:  The SUMO dump file containing the node movements
 * processors: A list of processors that should be added to every created node
 *
 * You can test the class with the following Shawn configuration file:
 *
 *   prepare_world edge_model=simple comm_model=disk_graph range=100
 *   node_movement mode=sumo net_file=eichstaett.net.xml dump_file=eichstaett-5T.dmp.xml processors=simple_app
 *   simulation max_iterations=43200
 *
 * This class reads the SUMO dump file block by block. One block contains all
 * movements within one time step. The movements are compared with those of the
 * previous time step to find new and deleted nodes. Such nodes are created
 * or deleted from the simulation. The block by block reading schema has the
 * advantage that only a small part of the SUMO dump file is kept in memory.
 * As a consequence, even large SUMO dump files (several gigabytes) can easily
 * be processed.
 */
class SumoNodeMovementCreator : public NodeMovementCreator
{
public:

   /**
    * Constructor
    */
   SumoNodeMovementCreator( SimulationController& sc );

   /**
    * Destructor
    */
   virtual
   ~SumoNodeMovementCreator();

   /**
    * Inherited from NodeMovementCreator. It returns the next node movement to be executed.
    * It is called whenever the movement controller needs a new command. The movement controller needs a new command as
    * long as next_movment does not return a NULL movement or a movement to be executed at a later point of time. Therefore,
    * next_movementreturns movements for each node at each round. When each node has been moved a MovementInfo for a NULL
    * node to be executed next round is returned.
    * The function also calls private functions to delete and add nodes.
    */
   virtual MovementInfo *
   next_movement();

   /*
    * Reset the input file
    */
   virtual void
   reset();

private:

   /**
    * SUMO net that is associated with this dump file. It is used
    * for coordinate transformations.
    */
   SumoNet net_;

   /**
    * Path to the SUMO dump file
    */
   std::string dump_file_;

   /**
    * Input stream for SUMO dump file
    */
   std::ifstream ifs_dump_file_;

   /**
    * Pointer to the simulation controller, initialized in constructor
    */
   SimulationController* sc_;

   /**
    * Last round that has been parsed
    */
   int last_parsed_round_;

   /**
    * Handle for processor factories to create processors specified in configuration file
    */
   std::vector<ProcessorFactoryHandle> processor_factories_;

   /**
    * Map that holds all current positions of all nodes
    */
   std::map<std::string, Vec> * map_current_positions_;

   /**
    * Map that holds all positions of all nodes at last round
    * This variable helps to detect whether a node has to be deleted, added or to define the parameters of the movement
    */
   std::map<std::string, Vec> * map_last_positions_;

   /**
    * If next_movement is executed for the first time at a Shawn round, this vetor is filled with all movements that
    * have to be executed this round.
    * All further calls of next_movement then just pick the next remaining movement until all movements are done
    */
   std::vector<MovementInfo*> remaining_movements_;

   /**
    * next_movement stores the round it is executed to this variable. So it can look by comparison with the world's
    *simulation_round() if it is executed for the first time at this round
    */
   int last_round_;

   /**
    * Help function to define private functions that are called when a new round starts (when next_movement() is executed
    * for the first time at a round)
    */
   void
   next_round();

   /**
    * Read data needed for current round from dumpfile.
    * Store this data to the map of current positions
    * Called by next_round()
    */
   void
   round_data_from_dumpfile();

   /**
    * Generate movement data for current round out of current and last positions
    * Store all movements to remaing movements vector
    */
   void
   generate_movements();

   /**
    * Add new nodes to simulation.
    * If a node appears in current positions map and does not in last positions map, it is added.
    */
   void
   add_new_nodes();

   /**
    * Remove nodes from simulation.
    * If a node appears in last positions map and does not in current positions map, it is deleted.
    */
   void
   delete_nodes();

   /**
    * Shawn stops simulation after 5 rounds with no active node. To prevent this, a dummy node is placed at the origin (0|0).
    * The dummy node has all processors that are defined in the configuration file! If the dummy node disturbs you, place
    * it somewhere else. It can be found by its label "Dummy node".
    */
   void
   create_dummy_node();

};

}

#endif /* ENABLE_SUMO */
#endif /* __SHAWN_APPS_SUMO_SUMO_NODE_MOVEMENT_CREATOR_H */
