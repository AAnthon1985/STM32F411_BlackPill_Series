# 01 OpenOCD
Basic setup of how to introduce the OpenOCD debug using the Cortex-Debug extension
## launch.json file
```
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Cortex Debug",
            "cwd": "${workspaceFolder}",
            "executable": "./02_openOCD/build/main.elf",
            "request": "launch",
            "type": "cortex-debug",
            "servertype": "openocd",
            // "device": "STM32F411CEU6",   // Not necessary
            "configFiles": [
                "interface/stlink-v2.cfg",
                "target/stm32f4x.cfg"
            ],
            "svdFile": "./02_openOCD/STM32F411.svd"
        }
    ]
}
```
