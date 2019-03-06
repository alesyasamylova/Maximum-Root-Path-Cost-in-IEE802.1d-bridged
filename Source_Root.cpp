/**************************************************************************************** 
** Project Name: Task_SPT 
** Program Name: Source_Root.cpp  
**
** Purpose:  to calculate maximum value of Root Path Cost of IEEE 802.1b bridged LAN.  
**           Each bridge has Root Path Cost. Root Path Cost is the Path Costs for each Bridge Port receiving frames forwarded from 
**           the Root on the least cost path to the Bridge 
**           The Root is the Bridge with the highest priority Bridge Identifier (i.e. min vaulue of it) 
** 
** Dependencies: the program uses STL library, and My_Dijkstry.h file 
** 
** Author:  Alesya Samylova 
** 
** Date: 12 DEC 2018 
** Update Date: 6 MAR 2019 
** 
** IDE: Microsoft Visual C++ 2010 Express 
** System Environment:  Windows 10 Home, Intel(R) Atom(TM) CPU Z3740 @ 1.33GHz, RAM 2.00GB, 32-bit, x-64-based processor 
**  
****************************************************************************************/ 

#include <iostream> 
#include <vector> 
#include <fstream> 
#include <string>  

#include "My_Dijkstry.h" 

using namespace std; 

//---------------------------------------------------------------------------------------------- 

struct Port 
{    
    int cost; 
    string connectedTo, portName; 

    Port()
	{ 
        cost=INT_MAX; 
        connectedTo=""; 
        portName=""; 
    } 
	
    Port (int weight, string name, string conTo) 
    {    
        cost = weight; 
        portName = name; 
        connectedTo = conTo; 
	} 
	
    void clear() 
	{ 
        cost=INT_MAX; 
        connectedTo.clear(); 
        portName.clear(); 
	} 

    void displayPort() 
	{   
	    cout << "Port Name: " << portName << endl; 
        cout << "Cost: "<< cost << endl; 
		cout << "Connected to: "<< connectedTo << endl; 
    } 

}; 
//------------------------------------------------------------- 

struct Bridge 
{ 
    int bridgeID; 
	vector <Port>   ports; // vector of ports 
    int rootPathCost; 
	string bridgeName; 

    Bridge() 
	{ 
	    bridgeID=INT_MAX; 
		rootPathCost = INT_MAX; 
    } 

    Bridge (int id, string name, vector<Port> vec) 
	{ 
	    bridgeID = id; 
		bridgeName = name; 
        ports.swap(vec); 
		rootPathCost = INT_MAX; 
	} 

    void clear() 
	{ 
        bridgeID=INT_MAX; 
        rootPathCost = INT_MAX; 
        bridgeName.clear(); 
        ports.clear(); 
    } 
	
    void displayBridge() 
	{     
        cout << "Bridge Name: " << bridgeName << endl; 
        cout << "Bridge Id: " << bridgeID << endl; 
        cout << "Root Path Cost: "<< rootPathCost << endl; 
		for (size_t i=0; i < ports.size(); i++)
		{
		    cout << "\n********   Port [" << i+1 << "]: *********\n";  
			ports[i].displayPort(); 
        } 
	} 
}; 
//-------------------------------------------------------------  

void displayNetwork (vector <Bridge> bridges) 
{ 
    cout<<"\n********************* Display of Network **********************\n"; 
	for (size_t i=0; i<bridges.size(); i++) 
	{ 
        cout<<"\n\n-------------- Bridge ["<< i +1 <<"] ----------------------------\n"; 
        bridges[i].displayBridge(); 
	} 
    cout<<"\n********************* End of Display ***************************\n";  

} 

void printRootPathsCosts(vector <Bridge> brds) 
{    
    for(size_t i = 0; i < brds.size(); i++) 
	{ 
        if (brds[i].rootPathCost != INT_MAX)
		{
			std::cout<<brds[i].bridgeName<<": "<<brds[i].rootPathCost<<"\n";
		}			
    } 

} 

// Function for finding Root Bridge Index 
size_t findRootIndex(vector <Bridge> brds) 
{ 
    //Root Bridge is Bridge with the lowest id 
    size_t N = brds.size(); // we can use size_t type, as it is alias for unsigned int   
	size_t root_index; 
    int min_id = INT_MAX; 
	
	for ( size_t i=0; i< N; i++) 
	{     
        if (brds[i].bridgeID < min_id) 
		{ 
	        root_index = i; 
            min_id = brds[i].bridgeID; 
		} 
    } 
    return root_index; 

} 
//---------------------------------------------------------------------------------------------- 

int main (int argc, char *argv[])  
{ 
    //******************************************************************************* 
    //****    UNIT 1: Define file name to be read                          
    //******************************************************************************* 

    try 
    { 
        char * file_name; 
        file_name = new char[36]; 
        cout << "\n----------------- START of the program -----------------\n"; 

		if (argc == 2)  
		{  
	        //********** CASE 1 - to READ File Name from Windows Console **** 
            file_name = argv[1]; 
            cout << ">>>> File name supplied from cmd is: \"" << file_name<<"\"\n"; 
		} 
		else 
		{ 
	        if (argc > 2)
			{
				throw "Too many parameters specified.\n Please use single file name without spaces\n";
			}				

            //********** CASE 2 - to READ File Name from Win32 Console for C++ *********** 
			cout << "Print the name of the file: "; 
			cin.getline(file_name, 36); 

			//********** CASE 3 - to GET STANDARD File Name *********** 
			if (!strlen (file_name)) //if we pressed Enter, filename is unspecified and we use test file 
			{  
			    cout <<  ">>>> NOTE: Standard input test file will be used\n"; 
				file_name = "test_bridges.txt"; 
			} 
        } 

        //********************************************************************************** 
        // *******    UNIT 2: Open the given file and parse its lines into vector bridges     
        //********************************************************************************** 
	    cout << "\n>>>> File to be opened: \"" << file_name << "\" <<<<"; 
        cout << "\n----------------------------------------------\n"; 

        ifstream myfile (file_name); 
        vector <Bridge> bridges; // Spanning Tree which represents the network  

        if (myfile.is_open()) 
        {    
           char str [256]; 
           Bridge brd;        // buff bridge element  
		   Port prt;          // buff port element 
           size_t num_of_bridges=0; // current number of bridges 

           cout << "CONTENT of the file \"" << file_name <<"\"\n"; 
		   cout<<"\n----------------- OPEN -----------------------\n"; 

           while (myfile.getline(str, 256)) 
		   {   
	            string line; 
		        line=str; 
                cout<<line<<endl; 
			
			    if (strlen(str)==0) // Skip any blank lines 
			    {
			        continue; 
			    }

                //************* CASE 1: Current line is BRIDGE declaration  *********************** 
		        if (line.find("[BRIDGE")!=string::npos && line.find("Port")==string::npos ) 
		        {       
	                //---------- Step 1: Create new Bridge by clearing value of buff Bridge ------ 
			        brd.clear(); 
			        num_of_bridges++; 

                   //---------- Step 2: Get Bridge Label - ignore the first [ and last char ]---- 
			        if(line.find("[")!=string::npos && line.find("]")!=string::npos) 
			        {    
		                brd.bridgeName = line.substr(1, line.size()- 2); 
                        //cout<<">> BRIDGE LABEL is : "<<brd.bridgeName<<endl;  
			        } 
				    else 
                    {				
		                throw std::invalid_argument("Invalid input file format. [,] should be present, i.e. [BRIDGE n]");   
				    } 
				   
                    //---------- Step 3: Get Bridge Id which is the 2nd line after Bridge declaration ---- 
                    myfile.getline(str,256); 
                    line=str; 
                    if(line.find("BridgeId = ")!=string::npos) 
                    {   
		                cout<<line<<endl; 
                        sscanf_s(line.substr(11).c_str(), "%d", &brd.bridgeID);  
				        //cout<<">> BRIDGE ID is : "<<brd.bridgeID<<endl; 
			        } 
			        else 
			        {
				        throw std::invalid_argument("Invalid input file format.\nAfter the [BRIDGE n] should be BridgeId line."); 
			        }
 
                    //---------- Step 4: PUT current BRIDGE to the our network Bridges ---- 
                    bridges.push_back(brd); 
		        }   
                //************* CASE 2: Current line is PORT declaration  *********************** 
                else if (line.find("Port")!=string::npos) 
			   {     
		            //---------- Step 1: Create new Port by clearing value of buff Port ------ 
				    prt.clear(); 

                    //---------- Step 2: Get Port Label - ignore the first [ and last char ] ------ 
				    if(line.find("[")!=string::npos && line.find("]")!=string::npos) 
                    {   
                        prt.portName=line.substr(1, line.size()- 2); 
                        //cout<<">> PORT LABEL is : "<<prt.portName<<endl; 
				    } 
                    else  
				    {
					    throw std::invalid_argument("Invalid input file format. [,] should be present, i.e. [BRIDGE n.Port xx]"); 
				    }

                    //---------- Step 3: Get Path Cost which is the 2nd line after Port declaration ---- 
                    myfile.getline(str,256); 
                    line=str; 
                    if (line.find("PathCost = ") != string::npos) 
				    {   
			            cout<<line<<endl; 
					    sscanf_s(line.substr(11).c_str(), "%d", &prt.cost); // convert string to integer 
				    } 
				    else 
				    {
					    throw std::invalid_argument("Invalid input file format.\n After the [Port n] should be \"PathCost = \" line."); 
				    }
				
				    //--------- Step 4: Get ConnectedTo for Port which is the 3rd line after Port declaration 
				    myfile.getline(str,256); 
				    line=str; 
				    if (line.find("ConnectedTo = ") != string::npos) 
				    {   
			            cout<<line<<endl; 
					    prt.connectedTo=line.substr(14);  
				    } 
				    else 
				    {
					    throw std::invalid_argument("Invalid input file format. \nAfter the \"PathCost = \" line should be \"ConnectedTo = \"."); 
				    }

                    //---------- Step 5: PUT current Port to the current Bridge ---- 
			        bridges[num_of_bridges - 1].ports.push_back(prt); 
			    } 
			    else  
			    {
				    throwstd::invalid_argument("Invalid input file format.\nThe example of input file format see in the specs\nPerhaps [ before Bridge is omitted"); 
			    }
            } 

            cout << "\n---------------- CLOSE ------------------------\n"; 
            myfile.close(); 
        } 
	    else 
	    {
			throw ("ERROR: Unable to open the file\n");  
	    }		
	
	    //displayNetwork(bridges); 
	
	    //**************************************************************************************
        //******* UNIT 3 - Create Matrix from Bridges n*n with Costs to implement oriented graph  
        //*************************************************************************************** 
        // We create matrix based on Bridges with Common LAN connection (i.e. they have Ports which points to the same LAN) 
        // Based on IEEE 802.1b it is assumed: there are no loops, for any two bridges only one common Edge exists 
        // by common Edge is meant two ports which points to the same LAN 

        size_t N = bridges.size(); 
        bool isCommonLANfound = false; 
        int **Weight = new int * [N]; // matrix of connection between bridges 
		
		for (size_t i = 0; i < N; i ++) // set all to zero assuming that there is no path for each bridge 
		{ 
		    Weight[i] = new int [N]; 
            for (size_t j = 0; j < N; j++)
			{
				Weight[i][j]=0; 
			}				
        } 

        for (size_t k=0; k < N; k++) 
		{   
	        for (size_t m=k+1; m < N; m++) 
            {    
		        isCommonLANfound = false; 
                for (size_t i=0; i < bridges[k].ports.size(); i++) 
				{ 
			        if (isCommonLANfound) 
						break; 

					for (size_t j=0; j < bridges[m].ports.size(); j++) 
					{ 
				        if (bridges[k].ports[i].connectedTo == bridges[m].ports[j].connectedTo) 
                        { 
                            Weight[m][k] = bridges[k].ports[i].cost; 
                            Weight[k][m] = bridges[m].ports[j].cost; 
                            isCommonLANfound = true; 
                            break; // assumed that there is no loops, i.e. to two LAN between two Brd 
						} 
					} 
				} 
			} 
		} 

        cout<<"\n>>>>>>> Matrix of Weight <<<<<<<<<<\n"; 
        for(size_t i=0; i < N; i++) 
        { 
            for(size_t j=0; j < N; j++) 
			{
				cout<<Weight[i][j]<<" "; 
			}
            cout<<endl; 

        } 

        //*************************************************************************************** 
        //**** UNIT 4 - Find out RootPathCosts and Choose Max value from them 
        //*************************************************************************************** 

        //--------------- Step 1: Find Root Bridge --------------------------- 
        size_t root_index=findRootIndex(bridges); //implementation of the func see at the top of the program 

        //--------------- Step 2:  Set for Root rootPathCost equals to zero ---- 
        bridges[root_index].rootPathCost = 0; 

		cout << "\n>>>>>> ROOT is \"" << bridges[root_index].bridgeName << "\" with \"BridgeID = " << bridges[root_index].bridgeID << "\" \n";  

        //--------------- Step 3: Find Root Path Cost for each Bridge ----------  
        //--------------   i.e. min paths from Root to each Bridge   -------- 
        int *distn= new int[N]; 

        //implementation and description of Dijkstra algorithm see in My_Dijkstry.h 
        Dijkstra(Weight,N,root_index,distn);  

        //set results from distn to bridges rootPathCost 
		for(size_t i = 0; i < N; i++) 
		{
			bridges[i].rootPathCost = distn[i]; 
		}

        //dispay rooPathCost for each Bridge 
		cout << "\n>>>>>> Root Path Cost: <<<<<<<<<<\n"; 
        printRootPathsCosts(bridges); 

        //--------------- Step 4: Choose the Max RootPathCost among Bridges ------ 

        int max=0; 
        size_t index_max; 

        for(size_t i=0; i<N; i++) 
        { 
            if(bridges[i].rootPathCost >= max && bridges[i].rootPathCost != INT_MAX)  
			{ 
		        max=bridges[i].rootPathCost; 
                index_max=i; 
			} 

        } 
		
        cout<<"\n>>>>>>> Max Root Path Cost: " << max <<endl; 
    } 
    catch (const char * p) 
    { 
        cerr<<"\n>>>>>>>>>>>> ERROR  <<<<<<<<<<<<"; 
        cerr<<endl<<p<<endl; 
		cerr<<">>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<\n"; 
        cerr<<"\nPress any key to exit ..."; 
        cin.get(); 
        return -1; 
    } 
	catch (const std::invalid_argument &inv) 
	{ 

        cerr<<"\n>>>>>>>>>>>> Format ERROR  <<<<<<<<<<<<\n"; 
        cerr << "INVALID VALUE ERROR: " << inv.what()<<endl; 
        cerr<<">>>>>>>>>>>>>>>>>>>>><<<<<<<<<<<<<<<<<<<<\n"; 
        cerr<<"\nPress any key to exit ..."; 
		cin.get(); 
		return -1; 
    } 
    catch(...) 
	{ 

        cerr<<"\n>>>>>>>>>>>> Unspecified ERROR  <<<<<<<<<<<<\n"; 
        cerr<<"\nPress any key to exit ..."; 
        cin.get(); 
        return -1; 
	} 

    cout << "\n--------------------- END of the program -------------------------\n"; 
    cout<<"\nPress any key to exit ..."; 
    cin.get(); 
    return 0; 
} 
