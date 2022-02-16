#include <iostream>
#include <stack>
#include <queue>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

//robot status
enum robot_status{
	stop,    //stop
	idle,    //idle
	picking, //picking a box from the pallet
	placing	 //placing a box on the pallet
};

//box size
enum box_size{
	small, //small box
	large, //large box
	nosize 
};


//Pallet class for pallet and conveyor queue loading boxes
class Pallet{ 
	public:
		queue<int> data; //storage for box 		 
	    int capacity;//maximum # of conveyer	
	public:
		Pallet(int capa):capacity(capa){}
		friend class Robot;	
		void pallet_load(int size)//load a box on the pallet
		 {		 	
		 	 if(data.size()<capacity)
		 	 {		 	   	
		 	   data.push(size);
			 }		 	 
		 	 else
		 	 {
		         cout<<"Fully loaded"<<endl;
        	   }		 	   		 	  
		 }			 	  
		int pallet_unload()//unload a box from the pallet
		 { 	
		    if(!data.empty())
		     {
		      int ans = data.front();
		      data.pop();
			  return ans;
			 }
			 else
			 {
			 	cout<<"Pallet is empty"<<endl;
			 	return nosize;
			 }	    	 
		  } 
		int check_size()//check the size of box that must be unloaded from queue
		{
		  return data.front();
		}  		
		bool is_empty() // check if the queue is empty
		{
		  return data.empty();
		} 
		bool is_full() // check if the queue is full
		{
		  return data.size()==capacity; 
		}
		int size()
		{
		   return data.size();	// check current filled data size
		}		 
};


//Robot class
class Robot{
	private:
        int status; // robot status						
        int box_size; // holding box size									
        bool holding; // holding box or not											
	public:
		Robot(int status, int box_size, bool holding):status(status), box_size(box_size), holding(holding){}			
		friend class Simulation;
		void place_pallet(int box, Pallet &P) //load a box on the pallet
		{		
		    //check status   	
		    if(status != idle)  return;		    
			
		    if(box == large)
		      cout<<"Place a large box on the pallet"<<endl;	
		    else
		      cout<<"Place a small box on the pallet"<<endl;	
		    
			status = placing;//placing a box 
			P.pallet_load(box);
			status = idle; 
		}			
		int pick_pallet(Pallet &P) // unload a box from the pallet
		{	
		
		    //check status   	
		    if(status != idle)  return nosize;	
		
		    //picking a box	
	        status = picking;
		    int box = P.pallet_unload();
		    
			if(box == large)
		   	  cout<<"Pick a large box from the pallet"<<endl;	    
		    else
		      cout<<"Pick a small box from the pallet"<<endl;	 
		    
			status = idle;
			return box;
		}				
		void place_conveyor(int box, Pallet &C) //load a box on the pallet
		{		
		    //check box size and statud;
		    if(box == nosize || status != idle) return;
		  
		    if(box == large)
		      cout<<"Place a large box on the conveyor"<<endl;
			else
			  cout<<"Place a small box on the conveyor"<<endl;	
		    
			status = placing;
			C.pallet_load(box);
			status = idle;
		}			
		int pick_conveyor(Pallet &C) // unload a box from the pallet
		{	    		    
		    //check status   	
		    if(status != idle)  return nosize;	
		
		    status = picking;
		    int box = C.pallet_unload();
		    
			if(box==large)
		      cout<<"Pick a large box from conveyor"<<endl;
		    else
		      cout<<"Pick a small box from conveyor"<<endl;
		    
			status = idle;
		    return box;
		}			
		void set_status(int sta)//set status of robot
		{
			status = sta;
		}		
		int get_status() // get status of robot
		{
			return status;
		}		
};

       
//simulation status
enum simulstatus{
	init,
	idl,
	normal,
	pause,
	exiit		
}; 


//simulation class
class Simulation{
	private:
	    int state;		
		bool proximity; //proximity sensor of_off		
		int pause_timer; //pause timer
		bool start_unloading; // when robot can unload a box from conveyor
		
					
	public:		
	    Simulation(int state, bool proximity, int pause_timer, bool start):state(state),
		           proximity(proximity), pause_timer(pause_timer), start_unloading(start){}
	 
	    void set_proximity(bool on_off, int timer) // proximity sensor setting
		{
			proximity = on_off;
			pause_timer = timer;
		} 	 
		int get_proximity() // get proximity sensor status
		{
			return proximity;
		}
		int get_pausetime() // get pause left pause time
		{
			return pause_timer;
		  } 
		void set_pausetime(int timer) // set pause timer
		{
			pause_timer = timer;
		}
		bool get_startunloading() // get if robot can start unloading box from conveyor
		{
			return start_unloading;
		}
		bool set_startunloading(bool start) // set flag to make robot can unload a box from conveyor
		{
			start_unloading = start;
		}
		int get_state()
		{
			return state;
			}	
		void set_state(int sta)
		{ 
			state = sta; 
			}	
};
        

	   
int main(int argc, char** argv) 
{ 	    
	vector<int> data = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};//20 box datae	of A pallet	
	Pallet A(20), B_small(10), B_large(10);	// Box will move from A pallet to B_small and B_large
	Pallet C(6); // Conveyor can carry maximum 6 boxes	
	Robot depalletizer(idle, nosize, false), palletizer(idle, nosize, false); // two robots are needed	
	Simulation simul(init, false, 0, false); // set proximity off, pause timer = 0.
	
	int status = 0, box = 0; 
	
		
	  while(1)
	  {
	  	  	status = simul.get_state();
				  
	  	    switch(status)
	  	     {
	  	     	case init://fill out the A pallet 	  	     	 
   	             for(int i=0; i<20; i++)
   	               A.pallet_load(data[i]);
	  	     	
	  	     	   simul.set_state(idle);			
				 break;
				  	   	     	
	  	     	case idle://conveyor need to be filled out 		   				   	
				    if(!C.is_full())  	   			
                     {                      
                       box = depalletizer.pick_pallet(A);
					   depalletizer.place_conveyor(box, C);  
			   	     }
				     else
					 	
				     {
				       simul.set_state(normal);	//if conveyor is full then goto normal, palletizer can pica a box from conveyor
					 } 
				 break;   	

				 
				   	
	  	     	case normal: // place a box on the conveyor and place a box on the new pallet from conveyor				   			   	     		  
				     if(!C.is_full() && !A.is_empty()) // load a box on the conveyor from A pallet 	   			
                     {                      
                       box = depalletizer.pick_pallet(A);
					   depalletizer.place_conveyor(box, C);  
			   	     }				   		   
					   
					    //pick and place a box from conveyor to new pallet
					      box = palletizer.pick_conveyor(C);		   	
			             
						  if(box == large)
		   	                palletizer.place_pallet(box, B_large);
		   	              else if(box == small)
		   	                palletizer.place_pallet(box, B_small);
		   	              else
		   	                {
		   	                 cout<<"No Box on the conveyor"<<endl; 
		   	                
							 if(A.is_empty()) // if all boxes are moved from A pallet to B_large and B_small
		   	                  simul.set_state(exiit);
						    }				               	     	
														
				     //Set Conveyor temporary stop   
                     if(A.data.size() == 13) //proximity sensor on to pause  
		             {
		  	          simul.set_proximity(true, 10); 					 	   	      
					  simul.set_state(pause);		
					  depalletizer.set_status(stop);
					  palletizer.set_status(stop);		  
		             } 
	  	     		
	  	     	 break;
	  	     		
	  	     	case pause://proximity sensor is one // stop all 
	  	     	    if(simul.get_pausetime() !=0) // check if thimer is expired.
	  	     	       {	     	       	 
	  	     	      	 simul.set_pausetime(simul.get_pausetime()-1);
	  	     	      	 cout<<"Proximity sensor on / Conveyor is stop"<<endl;
						 }
	  	     	       else
	  	     	       {
	  	     	       	  simul.set_proximity(false, 0);
	  	     	       	  simul.set_state(normal);	// if timer expired goto normal operation
						  depalletizer.set_status(idle);
					      palletizer.set_status(idle);						 	  	     	       	  
						  }	  
						  
				 break;
				
				case exiit:
				     goto here;
				 break;				 				
						  	     	
	  	     	default:
				  cout<<"Invalid state"<<endl;   	  	     	  	     		
	  	     	 break;
			   } 	
	  }        
   	
   	here:
   	       	
	return 0;
};
