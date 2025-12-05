FROM ubuntu:latest
RUN apt-get update && apt-get install -y util-linux 
WORKDIR /main
COPY containerlogic .
RUN chmod +x containerlogic
CMD ["/bin/bash", "containerlogic"]
