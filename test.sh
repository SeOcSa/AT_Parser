for entry in "$1"/*
do
echo "$entry"
  /home/zsofi/Egyetem/TD/AT_Parser-master/At-Parser/Debug/At-Parser "$entry"
done
