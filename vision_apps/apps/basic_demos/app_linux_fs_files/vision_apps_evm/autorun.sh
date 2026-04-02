export TEST_LIST=test_list.txt

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"
case $key in
    --ds)
    ds=1
    ;;
    --no_camera)
    no_camera=1
    ;;
    -h|--help)
    echo Usage: $0 [options]
    echo
    echo Options,
    echo --ds             Save Datasheets
    echo --no_camera      Only run the tests without camera support
    exit 0
    ;;
esac
shift
done
set -- "${POSITIONAL[@]}" # restore positional parameters

: ${ds:=0}
: ${no_camera:=0}

cd /opt/vision_apps
source ./vision_apps_init.sh

sleep 2

echo "Starting Test Automation"

while IFS= read -r line
do
    echo ${line}
    if [[ ${line:0:1} == "#" ]]
    then
        continue
    elif [[ -z $line ]]
    then
        continue
    else
        EXECUTION_CMD=`cut -d "|" -f1 <<< ${line}`
        DELAY=`cut -d "|" -f2 <<< ${line}`
        DS_ORIG=`cut -d "|" -f3 <<< ${line} | sed 's/\s*//g'`
        DS_DIR=`cut -d "|" -f4 <<< ${line} | sed 's/\s*//g'`
        DS_RENAME=`cut -d "|" -f5 <<< ${line} | sed 's/\s*//g'`
        DS_TITLE=`cut -d "|" -f6 <<< ${line}`
        CAMERAS=`cut -d "|" -f7 <<< ${line} | sed 's/\s*//g'`

        if [[ ${EXECUTION_CMD} == *"run_app_single_cam"* ]]
        then
            SINGLE_CAM=1
        else
            SINGLE_CAM=0
        fi

        if [ ${CAMERAS} -eq 0 ] || [ ${no_camera} -eq 0 ]
        then
            if [ ${ds} -eq 1 ]
            then
                # Use programmable delay for datasheet dump
                ./script.exp ${EXECUTION_CMD} ${DELAY} 1 ${CAMERAS} ${SINGLE_CAM}
                echo "Moving ${DS_ORIG} to datasheets/${DS_DIR}/${DS_RENAME}"
                mkdir -p datasheets/${DS_DIR}
                mv ${DS_ORIG} datasheets/${DS_DIR}/${DS_RENAME}
                sed -i -e "s/ Datasheet /${DS_TITLE}/" datasheets/${DS_DIR}/${DS_RENAME}
                bit8="8bit"
                bit16="16bit"
                ch4="4Ch"
                ch8="8Ch"
                if [[ ${DS_TITLE} == *"($bit8)"* ]] || [[ ${DS_TITLE} == *"($bit16)"* ]]
                then
                    if [[ ${DS_TITLE} == *"($bit8)"* ]]
                    then
                        replace=$bit8
                    else
                        replace=$bit16
                    fi
                    sed -i -e "s/_datasheet/_${replace}_datasheet/" datasheets/${DS_DIR}/${DS_RENAME}
                fi
                if [[ ${DS_TITLE} == *"($ch4)"* ]] || [[ ${DS_TITLE} == *"($ch8)"* ]]
                then
                    if [[ ${DS_TITLE} == *"($ch4)"* ]]
                    then
                        replace="4ch"
                    else
                        replace="8ch"
                    fi
                    sed -i -e "s/_datasheet/_${replace}_datasheet/" datasheets/${DS_DIR}/${DS_RENAME}
                fi
            else
                # Hard coding delay to 2 seconds for sanity testing (no datasheet dumps)
                ./script.exp ${EXECUTION_CMD} 2 0 ${CAMERAS} ${SINGLE_CAM}
            fi
        fi
    fi
done < ${TEST_LIST}

echo "Finished Test Automation"
