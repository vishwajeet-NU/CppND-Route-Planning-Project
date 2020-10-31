#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    start_node = &(m_Model.FindClosestNode(start_x,start_y));
    end_node = &(m_Model.FindClosestNode(end_x,end_y));

}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return (node->distance(*end_node));

}


void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {

    current_node->FindNeighbors();
    for(auto itr:current_node->neighbors){
        itr->parent = current_node;
        itr->g_value = current_node->g_value + itr->distance(*current_node);
        itr->h_value = CalculateHValue(itr);
        itr->visited = true;
        open_list.push_back(itr);

    }

}

bool RoutePlanner::comp(RouteModel::Node* one, RouteModel::Node* two){
        return((one->h_value+one->g_value) > (two->h_value+two->g_value));
}


RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(),open_list.end(),comp);
    RouteModel::Node *ret = open_list.back();
    open_list.pop_back()
    return ret;
}



std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found = {};
    std::cout<<"current_node final_path = "current_node->x<<" "<<current_node->y<<std::endl;
    // TODO: Implement your solution here.
    path_found.push_back(*current_node);
    while(current_node!=start_node){
        RouteModel::Node *pr = current_node->parent;
        distance+= current_node->distance(*pr);
        path_found.push_back(*pr);
        current_node = pr;
    }
    reverse(path_found.begin(),path_found.end());
    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}



void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    current_node = start_node;
    current_node->g_value = 0;
    current_node->h_value = CalculateHValue(current_node);
    current_node->parent = current_node;
    current_node->visited = true;
    open_list.push_back(current_node);
    // TODO: Implement your solution here.
    while (!open_list.empty())
    {
        current_node = NextNode();
        if(current_node->distance(*end_node) ==0 ){
            break;
        }
        AddNeighbors(current_node);

    }

    m_Model.path = ConstructFinalPath(current_node);

}