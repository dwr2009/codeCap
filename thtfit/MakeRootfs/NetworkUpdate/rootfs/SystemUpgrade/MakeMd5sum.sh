#!/bin/sh

function ExitProcess()
{
        local iArgCnt=$#
        local iProcessExitCode=$1;
        
        if [ ${iArgCnt} -eq 0 ]; then
                iProcessExitCode=0
        fi
        
        exit ${iProcessExitCode};
}

rm -f md5sum
ProcessExitCode=$?
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "Failed to delete md5sum(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum FilesToBackup.txt >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum FilesToDelete.txt >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum rootfs.tgz >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum Upgrade.sh >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum bolt.bin >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi

md5sum zImage >> md5sum
if [ ${ProcessExitCode} -ne 0 ]; then
        echo "md5sum err(${ProcessExitCode})"
        ExitProcess ${ProcessExitCode}
fi
