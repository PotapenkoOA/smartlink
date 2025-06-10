FROM ubuntu:latest
RUN apt-get update && apt-get install -y libstdc++6
RUN apt-get install -y libboost-all-dev

EXPOSE 8080

WORKDIR /app
COPY ./build/agent /app

ENTRYPOINT ["/bin/sh", "-c"]
CMD ["./agent" ]
