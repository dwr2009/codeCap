#!/bin/sh

PS3="Enter your option: "
select Option in "Normal" "HdmiHpdForce"
do
	case ${Option} in
		"Normal")
			rm -f TfUpgradeRootfs/SystemUpgrade/HardwareClass.txt
			ProcessExitCode=$?
			if [ ${ProcessExitCode} -ne 0 ]; then
				exit ${ProcessExitCode};
			fi
			break ;;
		"HdmiHpdForce")
			echo "HdmiHpdForce" > TfUpgradeRootfs/SystemUpgrade/HardwareClass.txt
			break ;;
		*)
			echo "Wrong selection" ;;			
	esac
done
