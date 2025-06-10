FROM ubuntu:latest
RUN apt-get update && apt-get install -y libstdc++6
RUN apt-get install -y libboost-all-dev

EXPOSE 8080

WORKDIR /app
COPY ./build/http_server /app
COPY ./settings/settings.json /app
COPY ./build/plugins/Res_200_Handler/libRes_200_Handler.so /app/plugins/libRes_200_Handler.so
COPY ./build/plugins/Res_405_Handler/libRes_405_Handler.so /app/plugins/libRes_405_Handler.so

ENTRYPOINT ["/bin/sh", "-c"]
CMD ["./http_server" ]
