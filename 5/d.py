import heapq
import copy

NO_PATH = 1000000
# FILE_PATH = "small.txt"         # 2
# FILE_PATH = "medium.txt"      # 5
# FILE_PATH = "large.txt"       # 26
FILE_PATH = "dijkstraData.txt" # ?
# TARGET_NODES = 

class Edge:
    def __init__(self, dest_node_id, weight):
        self.dest_node_id = dest_node_id
        self.weight = weight

class Node:
    def __init__(self, node_id):
        self.node_id = node_id
        self.edges = list()
        
    def add_edge(self, edge):
        self.edges.append( edge )

class Graph:
    def __init__(self):
        self.nodes = dict()

    def add_node(self, node):
        self.nodes[node.node_id] = node

    def get_node(self, node_id):
        return self.nodes[node_id]


graph = Graph()

for line in open(FILE_PATH):
    chunks = line.strip().split(' ')
    new_node_id = int(chunks.pop(0))

    new_node = Node(new_node_id)

    for chunk in chunks:
        edge_list = chunk.split(',')
        dest_node_id = int(edge_list[0])
        weight = int(edge_list[1])

        new_edge = Edge(dest_node_id, weight)
        new_node.add_edge(new_edge)

    graph.add_node(new_node)
        
class Path:
    def __init__(self):
        self.total_distance = 0
        self.node_ids = []

    def traveled_edge(self, edge):
        new_path = copy.deepcopy(self)
        new_path.total_distance += edge.weight
        new_path.node_ids.append(edge.dest_node_id)
        return new_path

    def report(self):
        return "Weight {} -- {}".format(self.total_distance, self.node_ids)

    @staticmethod
    def singleton(node_id):
        path = Path()
        path.node_ids = [ node_id ]
        return path

class Tree:
    def __init__(self):
        self.explored_node_ids = dict()
        self.candidate_paths = list()

    def add_path(self, path):
        print(path.report())
        new_node_id = path.node_ids[-1]
        self.explored_node_ids[new_node_id] = path
        print("Added Paths!")
        self.report_explored_paths()

    def consider_candidates(self):
        self.candidate_paths = []
        for explored_node_id, explored_path in self.explored_node_ids.items():
            # print(node_id)
            # print(explored_path.report())
            edges = graph.get_node(explored_node_id).edges
            for edge in edges:
                if edge.dest_node_id not in self.explored_node_ids:
                    new_path = explored_path.traveled_edge(edge) 
                    self.candidate_paths.append(new_path)

    def report_candidates(self):
        print("Candidate Paths:")
        self.report_paths(self.candidate_paths)

    def report_explored_paths(self):
        for node_id, path in self.explored_node_ids.items():
            print("{} :: ".format(node_id), path.report())

    def report_paths(self, paths):
        for path in self.candidate_paths:
            print(path.report())

tree = Tree()
tree.add_path(Path.singleton(1))
tree.consider_candidates()
tree.report_candidates()

while len(tree.candidate_paths) > 0:
    next_path = min(tree.candidate_paths, key=lambda path: path.total_distance)
    print("Next Candidate!", next_path.report())
    tree.add_path(next_path)
    tree.consider_candidates()

if FILE_PATH == "dijkstraData.txt":
    print( list( map(lambda node_id: tree.explored_node_ids[node_id].total_distance, [7,37,59,82,99,115,133,165,188,197]) ) )
    print( list( map(lambda node_id: tree.explored_node_ids[node_id].total_distance, [10,30,50,80,90,110,130,160,180,190]) ) )
