#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <set>

using namespace std;
// create link
struct Link {
    int startNode;
    int endNode;
    double travelTime;
    double capacity;
    double flow;
    double initialTravelTime;
};

// create node
struct Node {
    int id;
    vector<int> outgoingLinks;
};

// create demand
struct Demand {
    int origin;
    int destination;
    double demand;
};

// create links and demands vector
vector<Link> links;
vector<Node> nodes;
vector<Demand> demands;

// parse the input files
void parseNetworkFile(const string& filename){
    ifstream file(filename);
    string line;
    getline(file,line);

    while(getline(file, line)){
        istringstream iss(line);
        int startNode, endNode;
        double capacity, travelTime;

        iss >> startNode >> endNode >> travelTime >> capacity;
        // create and store links
        Link link = {startNode, endNode, travelTime, capacity, 0.0, travelTime}; // intial flow is zero
        links.push_back(link);

        // increase nodes size
        while(nodes.size() <= max(startNode, endNode)){
            Node node;
            nodes.push_back(node);
        }
        //update start node
        nodes[startNode].id = startNode;
        nodes[startNode].outgoingLinks.push_back(links.size() - 1);

    }
}

void parseDemandFile(const string &filename){
    ifstream file(filename);
    string line;
    // Skip the header line
    getline(file, line);

    while(getline(file, line)){
        istringstream iss(line);
        int origin, destination;
        double demand;

        iss>> origin>> destination >> demand;
        Demand demandStruct = {origin, destination, demand};
        demands.push_back(demandStruct);
    }
}

// create graph as Adjacency list with weights attached
vector<vector<pair<int,double> > >createGraph(){
    vector<vector<pair<int,double> > > Adj_list(nodes.size());
        for(Link& link : links){
            Adj_list[link.startNode].emplace_back(link.endNode, link.travelTime);
        }
        return Adj_list;
}


// finding shortest path
vector<int> dijkstra(int v, vector<vector<pair<int,double> > >&Adj_list, int source, int destination){
    // create set data structure
    set<pair<double,int> >st;
    // create distance arr initialised to 1e9
    vector<double> dist(v, 1e9);
    // create prev array
    vector<int>prev(v,-1);

    //initialization
    st.insert({0,source});
    dist[source] =0.0;

    // iterations
    while(!st.empty()){
        auto it = *st.begin();
        double dis = it.first;
        int node = it.second;
        st.erase(st.begin());

        for(auto neighbour: Adj_list[node]){
            int AdjNode = neighbour.first;
            double edgeWeight = neighbour.second;

            if(dis+ edgeWeight < dist[AdjNode]){
                //check if the node is already visited or not
                if(dist[AdjNode] != 1e9){
                    st.erase({dist[AdjNode], AdjNode});
                }
                dist[AdjNode] = dis+ edgeWeight;
                prev[AdjNode] =node;
                st.insert({dist[AdjNode], AdjNode});
            }
        }
    }

    //reconstruct the path
    vector<int>path;
    for(int i=destination; i!=-1; i = prev[i]){
        path.push_back(i);
    }
    reverse(path.begin(), path.end());

    return path;
}

void allOrNothingAssignment(vector<vector<pair<int,double>>>&Adj_list, vector<double> &temp_flow){
    for(Demand &demand: demands){
        vector<int>path = dijkstra(nodes.size(), Adj_list, demand.origin, demand.destination);
        for(int i=0;i<path.size()-1;i++){
            int u = path[i];
            int v = path[i+1];

            for(Link &link : links){
                if(link.startNode ==u && link.endNode==v){
                    temp_flow[&link - &links[0]] += demand.demand;
                    break;
                }
            }
        }
    }
}

// using Breau of Public Roads equation
void updateTravelTimes() {
    for (Link &link : links) {
        double alpha = 0.15;
        double beta = 4.0;
        // BPR funtion

        link.travelTime = link.initialTravelTime * (1 + alpha * pow(link.flow / link.capacity, beta));
    }
}

void msaTrafficAssignment(int iterations) {
    auto adj = createGraph();
    vector<double> temp_flow(links.size(), 0.0);

    for (int iter = 1; iter <= iterations; ++iter) {
        fill(temp_flow.begin(), temp_flow.end(), 0.0); // Reset temporary flow

        // Perform All-or-Nothing Assignment
        allOrNothingAssignment(adj, temp_flow);

        // Update flow using MSA
        double lambda = 1.0 / iter;
        for (int i = 0; i < links.size(); ++i) {
            links[i].flow = lambda * temp_flow[i] + (1 - lambda) * links[i].flow;
        }

        // Update travel times based on new flows
        updateTravelTimes();
    }
}


int main() {
    parseNetworkFile("network.dat");
    parseDemandFile("demand.dat");

    int iterations = 100;
    msaTrafficAssignment(iterations);

    for (const Link &link : links) {
        cout << "Link (" << link.startNode << " -> " << link.endNode << "): Flow = " << link.flow << ", Travel Time = " << link.travelTime << endl;
    }

    return 0;
}

