# ARP Protocol Exploration

The aim of the project is to explore the **ARP (Address Resolution Protocol)**.

The program intercepts multicast ARP requests from **Host A** that wants to know the MAC address of **Host B**.  
It then sends back a **spoofed MAC address** to Host A — one that Host A will treat as the address of Host B.