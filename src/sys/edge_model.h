/************************************************************************
 ** This file is part of the network simulator Shawn.                  **
 ** Copyright (C) 2004-2007 by the SwarmNet (www.swarmnet.de) project  **
 ** Shawn is free software; you can redistribute it and/or modify it   **
 ** under the terms of the BSD License. Refer to the shawn-licence.txt **
 ** file in the root of the Shawn source tree for further details.     **
 ************************************************************************/

#ifndef __SHAWN_SYS_EDGE_MODEL_H
#define __SHAWN_SYS_EDGE_MODEL_H

#include "sys/node_movements/movement_observer.h"
#include "sys/misc/node_change_listener.h"
#include <functional>

namespace shawn
{
   class World;
   class Node;
   class CommunicationModel;
   class Box;
   class Vec;

   typedef enum CommunicationDirection { CD_IN = 0, CD_OUT, CD_BIDI, CD_ANY } CommunicationDirection;

   class EdgeModel 
       : public MovementObserver,
         public NodeChangeListener
   {
   public:

	   
	   
	   template<typename NodeType> class CommunicationPattern 
			: public std::binary_function<NodeType, NodeType, bool> 
	  {
	  public:
		  CommunicationPattern() {}
		  virtual ~CommunicationPattern() {}
		  virtual CommunicationPattern<NodeType>* clone() = 0;
		  virtual bool operator()(NodeType&, NodeType&) = 0;
		  virtual std::string name() {return "untyped";}
	  };

	  template<typename NodeType> class BiDiCommPattern 
		  : public CommunicationPattern<NodeType>
	  {
	  public:
		  BiDiCommPattern(const EdgeModel& em) : em_(em) {}
		  virtual ~BiDiCommPattern() {}
		  virtual CommunicationPattern<NodeType>* clone() { return new BiDiCommPattern<NodeType>(em_);}
		  virtual bool operator()(NodeType& n1, NodeType& n2)
		  {
			  return em_.can_communicate_bidi(n1,n2);
		  }
		  virtual std::string name() {return "bidi";}
     private:
        const EdgeModel& em_;
	  };

	  template<typename NodeType> class InCommPattern 
		  : public CommunicationPattern<NodeType>
	  {
	  public:
		  InCommPattern(const EdgeModel& em) : em_(em) {}
		  virtual ~InCommPattern() {}
		  virtual CommunicationPattern<NodeType>* clone() { return new InCommPattern<NodeType>(em_);}
		  virtual bool operator()(NodeType& n1, NodeType& n2)
		  {
			  return em_.can_communicate_uni(n2,n1);
		  }
		  virtual std::string name() {return "in";}
     private:
        const EdgeModel& em_;
	  };

	  template<typename NodeType> class OutCommPattern 
		  : public CommunicationPattern<NodeType>
	  {
	  public:
		  OutCommPattern(const EdgeModel& em) : em_(em) {}
		  virtual ~OutCommPattern() {}
		  virtual CommunicationPattern<NodeType>* clone() { return new OutCommPattern<NodeType>(em_);}
		  virtual bool operator()(NodeType& n1, NodeType& n2)
		  {
			  return em_.can_communicate_uni(n1,n2);
		  }
		  virtual std::string name() {return "out";}
     private:
        const EdgeModel& em_;
	  };

	  template<typename NodeType> class AnyCommPattern 
		  : public CommunicationPattern<NodeType>
	  {
	  public:
		  AnyCommPattern(const EdgeModel& em) : em_(em) {}
		  virtual ~AnyCommPattern() {}
		  virtual CommunicationPattern<NodeType>* clone() { return new AnyCommPattern<NodeType>(em_);}
		  virtual bool operator()(NodeType& n1, NodeType& n2)
		  {
//			  	  std::cout << " AnyPattern.vor" << std::endl << std::flush;
					bool adj = ( em_.can_communicate_uni(n1,n2) || em_.can_communicate_uni(n2,n1));
//			  	  std::cout << " AnyPattern.nach" << std::endl << std::flush;

					return adj;
		  }
		  virtual std::string name() {return "any";}
     private:
        const EdgeModel& em_;
	  };

      template<typename NodeType> class AbstractIteratorHelper
      {
         //protected:
      public:
//		  CommunicationPattern<NodeType>* pattern_;
         CommunicationPattern<NodeType>& pattern_;
      public:
		  AbstractIteratorHelper(CommunicationPattern<NodeType>& p) : pattern_(p) {}
		  virtual ~AbstractIteratorHelper();
         virtual void init( void ) throw() = 0;
         virtual void next( void ) throw() = 0;
         virtual NodeType* current( void ) const throw() = 0;
         virtual AbstractIteratorHelper<NodeType>* clone( void ) const throw() = 0;
      };



   public:

      template<typename NodeType>
      class AbstractAdjacencyIterator
      {
      public:
         typedef AbstractIteratorHelper<NodeType> Helper;
         AbstractAdjacencyIterator();
         AbstractAdjacencyIterator( const AbstractAdjacencyIterator<NodeType>& );
         AbstractAdjacencyIterator( Helper* );
         ~AbstractAdjacencyIterator();

         AbstractAdjacencyIterator& operator++ ( void ) throw();
         NodeType& operator* ( void ) const throw();
         NodeType* operator -> ( void ) const throw();

         bool operator == ( const AbstractAdjacencyIterator<NodeType>& ) const throw();
         bool operator != ( const AbstractAdjacencyIterator<NodeType>& ) const throw();

         AbstractAdjacencyIterator& operator = ( const AbstractAdjacencyIterator<NodeType>& ) throw();
      private:
         // use prefix operator ("++it") only
//         AbstractAdjacencyIterator operator++ ( int );
         Helper* helper_;
      };


      typedef
      AbstractAdjacencyIterator<Node>
      adjacency_iterator;
      typedef
      AbstractAdjacencyIterator<const Node>
      const_adjacency_iterator;


      
      ///
      EdgeModel();
      ///
      virtual ~EdgeModel();
      ///
      virtual void set_world( World& ) throw();
      ///
      virtual void set_communication_model( const CommunicationModel& ) throw();
      ///
      World& world_w( void ) throw();
      ///
      const World& world( void ) const throw();
      ///
      const CommunicationModel& communication_model( void ) const throw();
      ///
      virtual void init( void ) throw();

      ///
      virtual int
      nof_adjacent_nodes( const Node&, CommunicationDirection d = CD_BIDI) 
         const throw() = 0;
      ///
      virtual const_adjacency_iterator
      begin_adjacent_nodes( const Node&, CommunicationDirection  d = CD_BIDI)
         const throw() = 0;
      ///
      virtual const_adjacency_iterator
      end_adjacent_nodes( const Node& )
         const throw() = 0;
      ///
      virtual adjacency_iterator
		  begin_adjacent_nodes_w( Node&, CommunicationDirection d = CD_BIDI)
         throw() = 0;
      ///
      virtual adjacency_iterator
      end_adjacent_nodes_w( Node& )
         throw() = 0;
      /// Just for completeness -- may query CommunicationModel!
      virtual bool
      can_communicate_bidi( const Node&, const Node& )
         const throw();
      /// 
      virtual bool
      can_communicate_uni( const Node& u, const Node& v )
         const throw();
      /// 
      virtual bool
      are_adjacent( const Node& u, const Node& v, CommunicationDirection d = CD_BIDI )
         const throw();

      ///
      virtual bool supports_mobility( void ) const throw() = 0;

	  virtual CommunicationPattern<const Node>* get_communication_pattern(CommunicationDirection d) const { return ccp[d];}
	  virtual CommunicationPattern<Node>* get_communication_pattern_w(CommunicationDirection d) { return cp[d];}

     ///@name methods inherited from World::NodeChangeListener
     ///@{
     virtual void node_added( Node& ) throw() = 0;
     virtual void node_removed( Node& ) throw() = 0;
     virtual void id_changed( int, int ) throw();
     virtual bool invalidate( void ) throw();
     ///@}

   private:
      World* world_;
      const CommunicationModel* comm_model_;
	  CommunicationPattern<const Node>* ccp[4];	  
	  CommunicationPattern<Node>* cp[4];
   };

}

#endif
/*-----------------------------------------------------------------------
 * Source  $Source: /cvs/shawn/shawn/sys/edge_model.h,v $
 * Version $Revision: 1.10 $
 * Date    $Date: 2007/01/02 15:37:20 $
 *-----------------------------------------------------------------------
 * $Log: edge_model.h,v $
 *-----------------------------------------------------------------------*/
