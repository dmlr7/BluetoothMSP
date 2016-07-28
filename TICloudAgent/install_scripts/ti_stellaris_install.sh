#! /bin/bash
# ========================================================================== 
# ti_stellaris_install.sh
# - install or uninstall 99-icdi.rules file in /etc/udev/rules.d directory.
# - copy libftd2xx library to /usr/lib/ and make a symbolic link.
#
# pass parameter "--install" to install the rules file, and setup libftdi.
# pass parameter "--uninstall" to remove the rules file, and remove libftdi. 
#
# This script needs to be executed as root.
#
if [ "${1}" = "" ];
then
    echo "Usage: ${0} --install or --uninstall"
    exit 1
fi

typeset RULE_FILE=/etc/udev/rules.d/99-icdi.rules

if [ "${1}" = "--install" ];
then
    echo "Install 99-icdi.rules in /etc/udev/rules.d directory"
    # remove the file if any
    rm -f ${RULE_FILE}

    # write to the file
    printf "ACTION!=\"add|change\", GOTO=\"openocd_rules_end\"\n" >> ${RULE_FILE}
    printf "SUBSYSTEM!=\"usb\", GOTO=\"openocd_rules_end\"\n" >> ${RULE_FILE}
    printf "ENV{DEVTYPE}!=\"usb_device\", GOTO=\"openocd_rules_end\"\n" >> ${RULE_FILE}

    printf "\n" >> ${RULE_FILE}
    printf "# PC Instruments\n" >> ${RULE_FILE}
    printf "ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"dc48\", MODE=\"777\"\n" >> ${RULE_FILE}

    printf "\n" >> ${RULE_FILE}
    printf "# TI Stellaris Evaluation Board (several)\n" >> ${RULE_FILE}
    printf "ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"bcd9\", MODE=\"777\"\n" >> ${RULE_FILE}

    printf "\n" >> ${RULE_FILE}
    printf "# TI Stellaris In-Circuit Debug Interface (ICDI) Board\n" >> ${RULE_FILE}
    printf "ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"bcda\", MODE=\"777\"\n" >> ${RULE_FILE}

    printf "\n" >> ${RULE_FILE}
    printf "# LMICDI\n" >> ${RULE_FILE}
    printf "ATTRS{idVendor}==\"1cbe\", ATTRS{idProduct}==\"00fd\", MODE=\"777\"\n" >> ${RULE_FILE}

    printf "LABEL=\"openocd_rules_end\"" >> ${RULE_FILE}
    printf "\n" >> ${RULE_FILE}

    # copy ftdi library to /usr/lib directory
    echo "Copy libftdi to /usr/lib directory"
    cp $(dirname $0)/../ccs_base/DebugServer/drivers/libftd2xx.so.1.1.12 /usr/lib/.
    # Create a symbolic link to the ftdi library
    echo "Creat symbolic link to ftdi library"
    ln -sf /usr/lib/libftd2xx.so.1.1.12 /usr/lib/libftd2xx.so
    exit 1
fi

if [ "${1}" = "--uninstall" ];
then
    echo "Remove 99-icdi.rules from /etc/udev/rules.d directory"
    # remove the file if any
    rm -f ${RULE_FILE}
   
    # remove libftd2xx
    echo "Remove libfd2xx from /usr/lib directory"
    rm -f /usr/lib/libftd2xx.so.1.1.12
    # remove the symbolic link
    echo "Remove symbolic link"
    rm -f /usr/lib/libftd2xx.so
    exit 1
fi
