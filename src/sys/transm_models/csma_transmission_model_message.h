#include "sys/event_scheduler.h"
#include "sys/misc/random/basic_random.h"
#include "sys/message.h"
namespace shawn{

	/**
	*@brief The structure for information of a destination node
	*
	*The structure includes the pointer of shawn::Node and a tag of this node 
	*@see shawn::Node
	*/
	struct msg_destination
		{
		shawn::Node* dest_node_;///<pointer to the destination node
		bool valid_;///<\c true if there is no collision at the node

		///Construction
		msg_destination( shawn::Node* pNode )
			{
			dest_node_ = pNode;
			valid_ = true;
			}
		};
	/**
	*@brief Message structure used in csma transmission model
	*
	*The structure includes the pointer of TransmissionModel::MessageInfo, its destinations, 
	*the delivery time and the remaining backoff time.
	* The csma_msg inherits EventScheduler::EventTag.
	* This is necessary for "adding" the messages to the EventScheduler
	*/

	class csma_msg : public EventScheduler::EventTag
		{
		public:

			TransmissionModel::MessageInfo *pmi_;

			///The vector which stores the neighbours of the source node
			std::vector<msg_destination*> destinations_;
			double deliver_time_;
			double backoff_;
			double duration_;
			bool sending_;
			int sending_attempts_;
			bool collision_;

			/**
			*@brief construction of structure csma_msg
			*
			*The deliver_time of the message is initialized as the sending time, and backoff is given 
			*a negative value to show that the message has not got any backoff time yet.
			*/
			csma_msg(){}
			csma_msg( TransmissionModel::MessageInfo* mi, double duration,double backoff):
			  pmi_(mi), duration_(duration), deliver_time_(mi->time_), sending_(false),
			  backoff_(random(0.001,backoff)), sending_attempts_(0), collision_(false)
				{
				}

			/**
			*@brief destruction of structure csma_msg
			*
			*Free the occupied memory of the message and its destinations
			*/
			~csma_msg()
				{
				delete pmi_;
				for( std::vector<msg_destination*>::iterator dest_it=destinations_.begin();
					dest_it!=destinations_.end(); dest_it++ )
					{
					delete (*dest_it);
					}
				}
			/**
			*@brief < Operator for csma_msg
			*@param msg Other message
			*/
			bool operator < ( csma_msg msg )
				{
				return deliver_time_ < msg.deliver_time_;
				}

			/**
			*@brief Push a destination node into the vector destination_
			*
			*@param pNode pointer to the destination node
			*/
			void push_new_destination( shawn::Node* pNode )
				{
				destinations_.push_back( new msg_destination(pNode) );
				}
			/**
			*@brief Returns a random double value within given bounds
			* @param lowerBound Random value's lowerBound
			* @param upperBound Random value's upperBound
			*/
			double random(double lowerBound, double upperBound) throw(){
				return (lowerBound + shawn::uniform_random_0i_1i()*(upperBound - lowerBound));
				}
			/**
			* @brief Should be set if message is sent.
			*/
			void setSending(){
				sending_ = true;
				}
			/**
			* Returns wether message will be send.
			* @ret bool
			*/
			bool isSending(){
				return sending_;
				}
			
		};
	}
