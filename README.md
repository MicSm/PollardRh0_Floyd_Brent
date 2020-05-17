Simulation of rho-Pollard method utilising the birthday paradox to find
secret argument. 

Currently this realisation uses simple multiplication P = k * G mod modulo,
where modulo should be prime number.

Also, it uses two different types of finding cycle algorythms: Floyd and 
Brent.

In practice, using Brent is more fast then using Floyd in terms of group 
operations, whereas "standard" rho-Pollard method uses only Floyd.

This sample can be easily extended for multiplication over Elliptic Curve,
where G is generator point of EC, k is choosen secret key (like private key 
in Bitcoin system) and P is known public key.
