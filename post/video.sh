mv ../build/out.h5 output
cd output
h5topng -t 0:999 -Zc dkbluered -a yarg  out.h5:Fields/Ez
ffmpeg -i out.t%03d.png out.mp4 -y
mpv out.mp4
