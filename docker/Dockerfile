FROM debian:buster

RUN apt-get update
RUN apt-get install -y vim less procps netcat-openbsd
RUN apt-get install -y nginx

RUN sed -i "s|user www-data;|user root;|g" /etc/nginx/nginx.conf
RUN rm /etc/nginx/sites-enabled/default

COPY ./conf/site1.conf /etc/nginx/conf.d/site1.conf
COPY ./conf/site2.conf /etc/nginx/conf.d/site2.conf

EXPOSE 8080
EXPOSE 8081

CMD ["nginx", "-g", "daemon off;"]


# docker build -t test .
# docker run -d -v /mnt/nfs/homes/alefranc/Documents/cursus42/5_webserv/webserv/www:/var/www/ -p 8080:8080 -p 8081:8081 test