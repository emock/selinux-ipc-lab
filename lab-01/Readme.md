
Let's first design how this could be implemented using SELinux.

Clearly, we need a label for:

- Domains:
    - client: client_t - manages the transition into the target type domain
    - server: server_t - manages the transition into the target type domain

What if we stopped, right here?

Let's investigate the ipc.te

ipc.te
```
policy_module(ipc, 0.1)

require {
	type unconfined_t;
}


type client_exec_t;
type client_t;


allow unconfined_t client_t: process transition;

allow client_t client_exec_t : file entrypoint;

type_transition unconfined_t client_exec_t:process client_t;

```


At the same time we assigned the client_exec_t type in ipc.fe to our binary

```
/usr/local/bin/client -- system_u:object_r:client_exec_t:s0
```

We confirm this using restorecon

```
dev@fedora:/usr/local/bin$ sudo restorecon -nv /usr/local/bin/*
[sudo] password for dev: 
Would relabel /usr/local/bin/client from unconfined_u:object_r:bin_t:s0 to unconfined_u:object_r:client_exec_t:s0
dev@fedora:/usr/local/bin$ sudo restorecon -v /usr/local/bin/*
restorecon: Could not set context for /usr/local/bin/client:  Permission denied

```


We cannot assign the type to our binary


```
dev@fedora:~$ seinfo -tclient_exec_t -x

Types: 1
   type client_exec_t;
```

Comparing with bin_t, we notice that this type inherits from a file_type

```
dev@fedora:~$ seinfo -tbin_t -x

Types: 1
   type bin_t alias { execmem_exec_t java_exec_t ls_exec_t mono_exec_t sbin_t unconfined_execmem_exec_t }, base_file_type, base_ro_file_type, entry_type, exec_type, file_type, non_auth_file_type, non_security_file_type;
```


