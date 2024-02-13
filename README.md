# etherflute
Networked musical instrument

# Hardware
Coming soon

# Compile and run
```
 platformio run            # build
 platformio run -t upload  # flash
 platformio device monitor # read serial output
```

# Send notes via UDP
```
  echo -n "f1c1a1b1" | nc -u -w1 <IP> <PORT>
```
