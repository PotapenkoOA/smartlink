FROM ubuntu:latest
RUN apt-get update && apt-get install -y libstdc++6
RUN apt-get install -y libboost-all-dev

EXPOSE 8087

WORKDIR /app
COPY ./build/http_server /app
COPY ./settings/settings.json /app
COPY ./build/plugins/Con_age_Handler/libCon_age_Handler.so /app/plugins/libCon_age_Handler.so
COPY ./build/plugins/Con_daytime_Handler/libCon_daytime_Handler.so /app/plugins/libCon_daytime_Handler.so
COPY ./build/plugins/Con_weekday_Handler/libCon_weekday_Handler.so /app/plugins/libCon_weekday_Handler.so
COPY ./build/plugins/Res_307_Handler/libRes_307_Handler.so /app/plugins/libRes_307_Handler.so
COPY ./build/plugins/Res_401_Handler/libRes_401_Handler.so /app/plugins/libRes_401_Handler.so
COPY ./build/plugins/Res_405_Handler/libRes_405_Handler.so /app/plugins/libRes_405_Handler.so

ENTRYPOINT ["/bin/sh", "-c"]
CMD ["./http_server" ]
