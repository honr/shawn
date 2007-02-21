/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_MISC_LOCALIZATION_PROCESSOR_TYPE_H
#define __SHAWN_SYS_MISC_LOCALIZATION_PROCESSOR_TYPE_H


namespace shawn
{

   // Abstract Interface
   /** Abstract Interface to act with SimulationTaskLocalizationAnchorPlacement.
    */
   class LocalizationProcessorType
   {

   public:

      enum ProcessorType
      {
         anchor, ///< anchors know their real position
         unknown ///< unknowns have to estimate their positions
      };

      ///@name ctor/dtor
      ///@{
      LocalizationProcessorType();
      virtual ~LocalizationProcessorType();
      ///@}


      ///@name processor type
      ///@{
      /** Set processor type. Namely anchor or unknown.
       */
      virtual void set_proc_type( const ProcessorType& ) throw();
      /** \return Type of processor
       */
      virtual const ProcessorType& proc_type( void ) const throw();
      /** \return \c true, if processor is anchor.
       */
      virtual bool is_anchor( void ) const throw();
      ///@}
      

   private:

      ProcessorType proc_type_;

   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/misc/localization/localization_processor_type.h,v $
 * Version $Revision: 1.4 $
 * Date    $Date: 2005/08/05 10:00:39 $
 *-----------------------------------------------------------------------
 * $Log: localization_processor_type.h,v $
 *-----------------------------------------------------------------------*/
