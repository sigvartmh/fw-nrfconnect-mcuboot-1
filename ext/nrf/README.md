# Building MCUBoot with nRF52840 CC310 enabled

## Pre-prerequisites

Install west from the zephyrproject requires Python 3
```
pip install --user west
```

Go outside the root folder and call

```
west init -l mcuboot
west update
```

## Building

make sure `root-ec-p256.pem` is set as the certificate and that `CONFIG_BOOT_SIGNATURE_TYPE_ECDSA_P256` is selected not `CONFIG_BOOT_SIGNATURE_TYPE_RSA` in `prj.conf` of `boot/zephyr`.
Since it defaults to tinycrypt you'll have to go into `menuconfig` and change the implementation selection to `cc310` or also set this in `prj.conf`.

```
mkdir build && cd build
cmake -GNinja -DBOARD=nrf52840_pca10056
ninja flash
```

Build a hello world example in zephyr and sign it with imgtool.py with the `root-ec-p256.pem` and flash it at `FLASH_AREA_IMAGE_0`.
