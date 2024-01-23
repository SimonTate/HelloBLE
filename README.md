# Hello BLE

Sample project for talking BLE with the [https://github.com/OpenBluetoothToolbox/SimpleBLE](SimpleBLE) library.


## Dependencies

Install SimpleBLE C++ version with:

```
git clone https://github.com/OpenBluetoothToolbox/SimpleBLE.git
cd SimpleBLE
cmake simpleble -B build_simpleble
cmake --build build_simpleble
cmake --install build_simpleble
```

You may need sudo for the install step.

## Compile

```
mkdir build/
cd build/
cmake ..
cmake --build .
```

## Run

```
cd build/
./hello-ble
```
