#!/bin/bash     

#PS3="Enter your option: "
Option=$1
#echo "Option:$Option"

#select Option in "Make MP7XX file system(Out/rootfs.tgz)" "Make Out/rootfs.tgz from Temp/rootfs"	\
#	"Produce Temp/rootfs" "Produce rootfs.tgz,Mp7xxDefCfg.tgz,Mp7xxLocalMedia.tgz"	\
#	"Produce the package NetworkUpdate" "Produce TfUpgradeRootfs"

#do
	case ${Option} in
		"Make MP7XX file system(Out/rootfs.tgz)"|1)
			./MakeMP7xxFs.sh
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		"Make Out/rootfs.tgz from Temp/rootfs"|2)
			./Make_rootfs_from_TempRootfs.sh
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		"Produce Temp/rootfs"|3)
			./MakeMP7xxFs.sh -NotCompress
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		"Produce rootfs.tgz,Mp7xxDefCfg.tgz,Mp7xxLocalMedia.tgz"|4)
			./Make_rootfs_DefCfg_LocalMedia.sh
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		"Produce the package NetworkUpdate"|5)
			./MakeNetworkUpdatePackage.sh
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		"Produce TfUpgradeRootfs"|6)
			./Produce_TfUpgradeRootfs.sh
			ProcessExitCode=$?
			exit ${ProcessExitCode}
			break ;;
		*)
			echo "Wrong selection" ;;
	esac
#done
