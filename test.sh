#!/usr/bin/env bash

echo image_DP87 HTTPS 219K
for i in `seq 1 20`;
do
    ./build/bin/simple_url https://images.metmuseum.org/CRDImages/dp/web-large/DP875552.jpg >  build/results/DP87.jpg
    ./build/bin/simple_c_url https://images.metmuseum.org/CRDImages/dp/web-large/DP875552.jpg > build/results/DP87c.jpg
done

echo Happy Image HTTPS 69K
for i in `seq 1 20`;
do
    ./build/bin/simple_url https://upload.wikimedia.org/wikipedia/commons/thumb/e/e0/SNice.svg/1200px-SNice.svg.png> build/results/happy.png
    ./build/bin/simple_c_url https://upload.wikimedia.org/wikipedia/commons/thumb/e/e0/SNice.svg/1200px-SNice.svg.png > build/results/happy_c.png
done

echo HTTP ~23K
for i in `seq 1 20`;
do
    ./build/bin/simple_url http://www.stroustrup.com/Bjarne2018.jpg > build/results/Bjarne.jpg
    ./build/bin/simple_c_url http://www.stroustrup.com/Bjarne2018.jpg > build/results/Bjarne_c.jpg
done

echo HTTP ~1MB
for i in `seq 1 20`;
do
    ./build/bin/simple_url http://images.cocodataset.org/annotations/image_info_test2014.zip > build/results/coco_anotations.zip
    ./build/bin/simple_c_url http://images.cocodataset.org/annotations/image_info_test2014.zip > build/results/coco_anotations_c.zip
done

echo HTTPS 222K stephen hawking
for i in `seq 1 20`;
do
    ./build/bin/simple_url https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Physicist_Stephen_Hawking_in_Zero_Gravity_NASA.jpg/1599px-Physicist_Stephen_Hawking_in_Zero_Gravity_NASA.jpg > build/results/stephen.jpg
    ./build/bin/simple_c_url https://upload.wikimedia.org/wikipedia/commons/thumb/0/08/Physicist_Stephen_Hawking_in_Zero_Gravity_NASA.jpg/1599px-Physicist_Stephen_Hawking_in_Zero_Gravity_NASA.jpg > build/results/stephen_c.jpg
done

echo HTTPS 14M Video firetruck
for i in `seq 1 20`;
do
    ./build/bin/simple_url https://upload.wikimedia.org/wikipedia/commons/transcoded/3/3d/Fire_training_alarm_in_KarRC_RAS_2018.ogv/Fire_training_alarm_in_KarRC_RAS_2018.ogv.480p.webm > build/results/firetruck.webm
    ./build/bin/simple_c_url https://upload.wikimedia.org/wikipedia/commons/transcoded/3/3d/Fire_training_alarm_in_KarRC_RAS_2018.ogv/Fire_training_alarm_in_KarRC_RAS_2018.ogv.480p.webm > build/results/firetruck_c.webm
done


for i in `seq 1 20`;
do
    ./build/bin/simple_url http://images.cocodataset.org/zips/val2017.zip > build/results/val2017.zip
    ./build/bin/simple_c_url http://images.cocodataset.org/zips/val2017.zip > build/results/val2017_c.zip
done

#echo Large files ~6GB
#for i in `seq 1 20`;
#do
    #./build/bin/simple_url http://images.cocodataset.org/zips/test2014.zip > build/results/coco.zip
    #./build/bin/simple_c_url http://images.cocodataset.org/zips/test2014.zip > build/results/coco_c.zip
#done
