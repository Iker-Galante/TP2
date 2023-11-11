# Check if docker TPEEspecial exists
if [ ! "$(docker ps -a -q -f name=TPEEspecial)" ]; then
    # Create docker TPEEspecial
    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name TPEEspecial agodio/itba-so:1.0
fi

docker start TPEEspecial
docker exec -it TPEEspecial make clean -C/root/Toolchain
docker exec -it TPEEspecial make clean -C/root/
docker exec -it TPEEspecial make -C/root/Toolchain
docker exec -it TPEEspecial env GCC_COLORS="error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01" make -C/root/
docker exec -it TPEEspecial chmod -R a+wx /root/
# docker stop TPEEspecial
bash run.sh
