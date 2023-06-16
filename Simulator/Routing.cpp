#include "Routing.h"

vector<NodeInfo> Routing::forward(Message &s) {
    vector<NodeInfo> ret = {};
    vector<NodeInfo> infos= s.rpath[0];
    for (int i=0; i<infos.size(); ++i) {
//        vector<CubeNode *> temp_dsts;
//        for (int k=0; k<infos[i].dsts.size(); ++k){
//            temp_dsts.push_back((*cube)[infos[i].dsts[k]]);
//        }
//        (*cube)[infos[i].cur] -> temp_dsts;
        vector<NodeInfo> temp_infos = forward_one(s, infos[i]);
        for (int k=0; k<temp_infos.size(); ++k) {
            ret.push_back(temp_infos[k]);
        }
    }
    return ret;
}

vector<NodeInfo> Routing::forward_one(Message &s, NodeInfo cur_info) {
    vector<NodeInfo> ret;
    if (cur_info.dsts.empty()) {
        ret.push_back(cur_info);
    } else {
        
    }
    return ret;
}
