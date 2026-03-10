sig Node {neighbour: set Node}

fact no_self_ref
{
    no n: Node | n in n.neighbour
}

fact undirected
{
    no n1,n2: Node | n1 in n2.neighbour and n2 not in n1.neighbour 
}

fact connected
{
    all n:Node | some n.neighbour
}

pred Show {}

run Show for 8

pred Tree [tr: Node -> set Node]
{
    all n1,n2:Node | n2 in n1.^tr
}

run Tree for 8