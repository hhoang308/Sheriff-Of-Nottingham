This is source code for server side of the game "Sheriff of Nottingham".

I prefer you to create an free account in AWS EC2 to host this game. After that, all of your friends can connect to it without your permission.

## Precondition
### Install dependencies
```
sudo apt-get update
sudo apt-get install build-essential cmake pkg-config
```
### Install ENet

```
# Download the ENet source code
wget http://enet.bespin.org/download/enet-1.3.15.tar.gz
# Extract the tarball
tar -xzf enet-1.3.15.tar.gz
cd enet-1.3.15
# Configure and build
./configure
make
# Install
sudo make install
# Verify the installation
ls /usr/local/lib | grep enet
ls /usr/local/include | grep enet
```

## How to build the project
```
# Clone the project
git clone https://github.com/your-repo/SheriffOfNottinghamServer.git
cd SheriffOfNottinghamServer
# Configure the project
mkdir build
cd build
# Build the project
cmake ..
make
# Run the server
./SheriffOfNottinghamServer
```

