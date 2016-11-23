#!/bin/sh
# $1 is tag name

tagname=$1
WORK_DIR=${PWD}

ProcessExitCode=0
${WORK_DIR}/MainMenu.sh 3
ProcessExitCode=$?
sleep 5
if [ $ProcessExitCode -ne 0 ];then
        echo "Err:cory rootfs failed....."
fi

${WORK_DIR}/MainMenu.sh 4
ProcessExitCode=$?
sleep 5
if [ $ProcessExitCode -ne 0 ];then
        echo "Err:make rootfs failed....."
fi

${WORK_DIR}/MainMenu.sh 5
ProcessExitCode=$?
sleep 5
if [ $ProcessExitCode -ne 0 ];then
        echo "Err:make Network rootfs failed....."
fi

if [ -f ${WORK_DIR}/Out/SystemUpgrade.tgz ];then
	rm ${WORK_DIR}/Out/SystemUpgrade.tgz
fi

if [ -f ${WORK_DIR}/NetworkUpdate/rootfs/SystemUpgrade.tgz ];then
	cp ${WORK_DIR}/NetworkUpdate/rootfs/SystemUpgrade.tgz ${WORK_DIR}/Out/ -rf
fi

if [ -z $tagname ];then
     echo "ERR:Tag is NULL,Please input tag." 
     exit
fi

cd $PRJ_TOP_DIR
if [ -f $PRJ_TOP_DIR/.repo/repo/repo ];then
        .repo/repo/repo  forall -c "pwd;git tag $tagname"
        ProcessExitCode=$?
        if [ $ProcessExitCode -ne 0 ];then
                echo "ERR:git tag failed"
                if [ $ProcessExitCode -eq 128 ];then
                        echo "ERR:tag have alreadly exist.delete $tagname now!"                 
                fi
                exit
        fi
	echo "git tag success."	

	.repo/repo/repo  forall -c "pwd;git push --tag"
	ProcessExitCode=$?
	if [ $ProcessExitCode -ne 0 ];then
		echo "ERR:git push tag failed."
	fi

	echo "git push tag success."

else
        echo "Err:Do not have $PRJ_TOP_DIR/.repo/repo/repo exec file."
        exit
fi

cd $WORK_DIR






