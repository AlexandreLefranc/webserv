FROM debian:buster

RUN apt-get update
RUN apt-get install -y vim less procps netcat-openbsd
RUN apt-get install -y nginx

RUN sed -i "s|user www-data;|user root;|g" /etc/nginx/nginx.conf
RUN rm /etc/nginx/sites-enabled/default

# COPY ./conf/site1.conf /etc/nginx/conf.d/site1.conf
# COPY ./conf/site2.conf /etc/nginx/conf.d/site2.conf

EXPOSE 8001
EXPOSE 8002

CMD ["nginx", "-g", "daemon off;"]


# docker buildx build -t test .
# docker run -d -v $PWD/../www:/var/www/ -v $PWD/conf:/etc/nginx/conf.d -p 8001:8001 -p 8002:8002 test