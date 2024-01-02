#------------------------------------------------------------------
# Author: ZhangYuehao
# Email: yuehaozhang@njtech.edu.cn
# Zhihu: https://www.zhihu.com/people/bu-meng-cheng-kong-46/posts
# GitHub: https://github.com/ZhangAilan
#-------------------------------------------------------------------
# Date: 2024/01/01
# Function:
#   Create random nodes and edges for testing
#-------------------------------------------------------------------
import random
import numpy as np


#generate random nodes and coordinates
num_nodes=50
x_range=(30,1450)
y_range=(0,800)

nodes=[]
for i in range(num_nodes):
    node_name='N'+str(i+1)
    x=random.uniform(*x_range)
    y=random.uniform(*y_range)
    y=800-y
    nodes.append([node_name,x,y])
 

#generate random edges and weights
num_edges=700
edges=[]

#ensure each node is used at least once
for i,node1 in enumerate(nodes):
    node2=random.choice(nodes)
    while node1==node2:
        node2=random.choice(nodes)
    edge_name='E'+str(i+1)
    weight=random.uniform(0,10)
    edges.append([edge_name,node1[0],node2[0],weight])

#add remaining random edges
for i in range(len(nodes),num_edges):
    node1,node2=random.sample(nodes,2)
    edge_name='E'+str(i+1)
    weight=np.sqrt((node1[1]-node2[1])**2+(node1[2]-node2[2])**2)
    edges.append([edge_name,node1[0],node2[0],weight])


#write data to file
with open('Dijkstra_Algorithm_Dynamic_Demonstration/nodes.txt','w') as f:
    for node in nodes:
        f.write(f'{node[0]} {node[1]} {node[2]}\n')

with open('Dijkstra_Algorithm_Dynamic_Demonstration/edges.txt','w') as f:
    for edge in edges:
        f.write(f'{edge[0]} {edge[1]} {edge[2]} {edge[3]}\n')

print("Done!!!")