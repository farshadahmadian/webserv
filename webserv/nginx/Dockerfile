FROM debian:bullseye

RUN apt-get update -y && apt-get install -y \
	nginx \
	nano

COPY conf/default.conf /etc/nginx/sites-available/
RUN ln -s /etc/nginx/sites-available/default.conf /etc/nginx/sites-enabled/
RUN rm -rf /etc/nginx/sites-enabled/default /etc/nginx/sites-available/default

ENTRYPOINT [ "/usr/sbin/nginx" ]
CMD [ "-g", "daemon off;" ]
# nginx -t can be used to test config file
