# geo-routing

In this bonus assignment you will implement a routing protocol (Geographic Forwarding GF). Geographic Forwarding (GF) is a greedy routing protocol. GF assumes that each nodes knows each location. When a node M [located at (xM, yM)] receives a packet for node D [located at (xD, yD)], node M will route this packet to its neighbor N [located at (xN , yN )] for which the distance d((xN , yN ) to (xD, yD)) is the smallest among all M’s neighbors. Please name your executable gf. Your compiled program will be invoked as follows:

$gf <input.file> <source node> <destination node> <radio range>

for example:

$gf input.file 1 5 15

where the input.file contains:

1  0  0

2 10 0

3 20 0

4 10 0

5 10 10

6 20 10
