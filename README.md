# ARP Protocol Exploration

The aim of the project is to explore the **ARP (Address Resolution Protocol)**.

The program intercepts multicast ARP requests from **Host A**, which wants to know the MAC address of **Host B**.  
It then sends back a **spoofed MAC address** to Host A — one that Host A will treat as the address of Host B.

## How to Test the Program

> 🐳 Docker is required to run this setup.

1. **Start the containers**  
   In the repository's directory, run:
   ```bash
   docker compose up --build
   ```
   This will create **3 running containers** joined to a dedicated network.

2. **List running containers**  
   ```bash
   docker ps
   ```
   Use this to find the names of the 3 containers.

3. **Connect to the containers**  
   From 3 different terminal windows, run:
   ```bash
   docker exec -ti <container_name> sh
   ```
   The working directory inside the container is already set to the application folder containing the executable.

4. **Check Docker networks**  
   In a separate terminal, run:
   ```bash
   docker network ls
   ```
   Find the name of the project network. It will be named like `*_malcolm_arp_test_network`.

5. **Inspect the Docker network**  
   Use this to get the IP addresses of the containers:
   ```bash
   docker network inspect *_malcolm_arp_test_network
   ```

6. **Run the ARP spoofing app**  
   Inside the **first container**, run:
   ```bash
   ./ft_malcolm <ip1> <mac1> <ip2> <mac2>
   ```
   Explanation of arguments:
   - `ip1`: IP address of Host B
   - `mac1`: Spoofed MAC to pretend as Host B
   - `ip2`: IP address of Host A
   - `mac2`: MAC address of Host A

   Host A wants to find out the MAC address for Host B, so it sends a multicast ARP request for `ip1`.  
   The program replies with a fake MAC address (`mac1`) for that IP.

7. **Verify ARP table (before)**  
   In the **second container**, run:
   ```bash
   arp -a
   ```
   No output should appear yet — the container doesn't know other MACs yet.

8. **Monitor ARP requests**  
   In the **third container**, run:
   ```bash
   tcpdump -i eth0 arp
   ```
   This starts capturing ARP packets, to verify that Host #2 sends the multicast ARP request.

9. **Trigger ARP request**  
   In the **second container**, run:
   ```bash
   ping -c 1 <ip1>
   ```
   This triggers the OS to send an ARP request to resolve the MAC for `ip1`.

10. **Observe app output**  
    In the **first container**, watch the application output.  
    It should say something like:
    ```
    received ARP request, send spoofed mac, please wait.
    ```

11. **Verify ARP table (after)**  
    In the **second container**, run:
    ```bash
    arp -a
    ```
    You should now see that `ip1` is associated with `mac1` — the spoofed MAC.
