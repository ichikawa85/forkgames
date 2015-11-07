# Makefile for cloud
server: cloud_server.c
	cc -Wall cloud_server cloud_server.c acl.c common.c -lwrap

client: cloud_client.c
	cc -Wall cloud_client cloud_client.c acl.c common.c -lwrap

