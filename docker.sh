# Check if docker TPEEspecial exists
if [ ! "$(docker ps -a -q -f name=TPSO)" ]; then
    # Create docker TPEEspecial
    docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -ti --name TPSO agodio/itba-so-multi-platform:3.0
fi

docker start TPSO
docker exec -it TPSO make clean -C/root/Toolchain
docker exec -it TPSO make clean -C/root/
docker exec -it TPSO make -C/root/Toolchain
docker exec -it TPSO env GCC_COLORS="error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01" make -C/root/
docker exec -it TPSO chmod -R a+wx /root/
# docker stop TPSO
bash run.sh
