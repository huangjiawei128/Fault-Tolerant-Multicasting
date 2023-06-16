#include "Routing.h"

vector<NodeInfo> Routing::forward(Message &s) {
    vector<NodeInfo> ret = {};
    vector<NodeInfo> infos= s.rpath[0];
    for (int i=0; i<infos.size(); ++i) {
        vector<CubeNode *> temp_dsts;
        for (int k=0; k<infos[i].dsts.size(); ++k){
            temp_dsts.push_back((*cube)[infos[i].dsts[k]]);
        }
        vector<NodeInfo> temp_infos = forward(s, (*cube)[infos[i].cur], temp_dsts);
        for (int k=0; k<temp_infos.size(); ++k) {
            ret.push_back(temp_infos[k]);
        }
    }
    return ret;
}

vector<NodeInfo> Routing::forward(Message &s, CubeNode *cur, vector<CubeNode *> dsts) {
    return {};
}
