echo Updating EHS6
echo Copying JAR 
python python/module_downloader.py -p %1 -b 115200 mqtt/dist/ThingsMQTT.jar ThingsMQTT.jar
echo Copying JAD
python python/module_downloader.py -p %1 -b 115200 mqtt/dist/ThingsMQTT.jad ThingsMQTT.jad
echo Stop, Install, Starting new module
python python/reinstall.py ThingsMQTT.jad -p %1
