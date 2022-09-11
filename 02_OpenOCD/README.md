# 01 OpenOCD
Basic setup of how to introduce the OpenOCD debug using the Cortex-Debug extension
## launch.json file
- Remember to have the Cortex-Debug extension installed and enabled (check the installation check-list from the STM32F411_BlackPill_Series level)
- Create a lunch.json file within your .vscode folder. Copy and paste the following text into your .json file
    - Then press F5 in VSCode to start your debug application (Remember to have compiled the latest version of your .elf file)
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
