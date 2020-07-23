# Battery Monitor  
Simple battery monitor for linux using `libnotify`  

## Installation
Clone the repo  
```
git clone https://github.com/nikhilmtomy/battery-monitor.git
```
Install `libnotify`  
- Debian / Ubuntu  
  ```
  sudo apt update -y && sudo apt install -y libnotify-dev
  ```
- Arch  
  ```
  sudo pacman -Sy && sudo pacman -S libnotify
  ```
Install  
```
cd battery-monitor
make && sudo make install
```