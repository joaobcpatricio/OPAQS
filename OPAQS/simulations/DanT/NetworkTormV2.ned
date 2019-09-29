package opaqs.simulations.DanT;

import opaqs.*;

network NetworkTormV2
{
    parameters:
        int numNodes = default(20);

        // statistics
        // from application layer
        // stat: liked data bytes received by all nodes
        @statistic[likedDataBytesReceivedSum](source=sum(likedDataBytesReceived);record=max,last,vector);

        // stat: non-liked data bytes received by all nodes
        @statistic[nonLikedDataBytesReceivedSum](source=sum(nonLikedDataBytesReceived);record=max,last,vector);

        // stat: duplicate (liked & non-liked) data bytes received by all nodes
        @statistic[duplicateDataBytesReceivedSum](source=sum(duplicateDataBytesReceived);record=max,last,vector);

        // stat: liked data bytes that are possible to be received by all nodes
        @statistic[likedDataBytesReceivableByAllNodesSum](source=sum(likedDataBytesReceivableByAllNodes);record=max,last,vector);

        // stat: non-liked data bytes that are possible to be received by all nodes
        @statistic[nonLikedDataBytesReceivableByAllNodesSum](source=sum(nonLikedDataBytesReceivableByAllNodes);record=max,last,vector);

        // stat: delivery delay of data (liked and non-liked) by all nodes
        @statistic[dataDelay](source=dataDelay;record=max,min,mean,vector,histogram);

        // stat: delivery ratio of data (liked and non-liked) by all nodes
        @statistic[deliveryRatio](source=sum(totalDataBytesReceived)/sum(totalDataBytesReceivableByAllNodes);record=max,min,vector);


        // from forwarding layer -> epidemic routing
        // stat: data bytes received by all nodes in the network 
        @statistic[dataBytesReceivedSum](source=sum(dataBytesReceived);record=max,last,vector);
        
        // stat: summary vectors bytes received by all nodes
        @statistic[sumVecBytesReceivedSum](source=sum(sumVecBytesReceived);record=max,last,vector);

        // stat: data request bytes received by all nodes
        @statistic[dataReqBytesReceivedSum](source=sum(dataReqBytesReceived);record=max,last,vector);

        // stat: total bytes received by all nodes
        @statistic[totalBytesReceivedSum](source=sum(totalBytesReceived);record=max,last,vector);

        
		// from wireless layer
		// stat: size of nodes in the wireless neghbourhood
		@statistic[neighbourhoodSize](source=neighbourhoodSize;record=min,max,mean,vector);
		
    submodules:

        // nodes of the network
        node[numNodes]: OPSNode {
            nodeID = index;
        }

}
