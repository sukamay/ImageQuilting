if [ $# -lt 4 ];
then
echo 'usage:'$0' [bin_path] [img_folder_path] [output_folder_path] [prefix]'
exit 1;
fi
for img in $(ls $2)
do
# echo $1 -input $2$img -output $3${img/test/$4}
# $1 -input $2$img -output $3${img/test/$4}
echo '<img src="./imgs/'${img}'" width=180/>'
echo '<img src="./outputs/'${img/test/texture}'" width=360/>'
echo '<br> <br>'
done 