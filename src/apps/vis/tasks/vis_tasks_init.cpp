/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004,2005 by  SwarmNet (www.swarmnet.de)             **
 **                         and SWARMS   (www.swarms.de)               **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the GNU General Public License, version 2.      **
 ************************************************************************/
#include "../buildfiles/_apps_enable_cmake.h"
#ifdef ENABLE_VIS

#include "apps/vis/tasks/vis_tasks_init.h"
#include "apps/vis/tasks/vis_create_task.h"
#include "apps/vis/tasks/vis_single_snapshot.h"
#include "apps/vis/tasks/vis_external_animation.h"
#include "apps/vis/tasks/vis_simple_camera_task.h"
#include "apps/vis/tasks/vis_create_group_task.h"
#include "apps/vis/tasks/vis_group_add_task.h"
#include "apps/vis/tasks/vis_create_edges.h"
#include "apps/vis/tasks/vis_create_edges_tag.h"
#include "apps/vis/tasks/vis_create_estimated_edges.h"
#include "apps/vis/tasks/vis_create_graphics.h"
#include "apps/vis/tasks/vis_tag_sampletask.h"
#include "sys/simulation/simulation_controller.h"
#include "sys/simulation/simulation_task_keeper.h"


namespace vis
{

  void init_vis_tasks( shawn::SimulationController& sc )
  {
     sc.simulation_task_keeper_w().add( new VisualizationTaskCreate );
     sc.simulation_task_keeper_w().add( new SingleSnapshotTask );
     sc.simulation_task_keeper_w().add( new ExternalAnimationTask );
     sc.simulation_task_keeper_w().add( new SimpleCameraTask );
     sc.simulation_task_keeper_w().add( new CreateGroupTask );
     sc.simulation_task_keeper_w().add( new GroupAddTask );
     sc.simulation_task_keeper_w().add( new CreateEdgesTask );
     sc.simulation_task_keeper_w().add( new CreateEdgesTagTask );
     sc.simulation_task_keeper_w().add( new CreateEstimatedEdgesTask );
     sc.simulation_task_keeper_w().add( new CreateGraphicsTask );
     sc.simulation_task_keeper_w().add( new TagSampleTask );
  }

}

#endif