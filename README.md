❯ wasme build cpp  -t webassemblyhub.io/rksmannem/add-header-cpp.wasm:v0.1.0 .   
❯ wasme list  
❯ wasme push webassemblyhub.io/rksmannem/add-header-cpp.wasm:v0.1.0                 
❯ wasme list --search rksmannem

1. wasme deploy envoy webassemblyhub.io/rksmannem/add-header-cpp.wasm:v0.1.0 --envoy-image=istio/proxyv2:1.5.1
   » curl -kv -H "name:rama" localhost:19000/

2. Now Deploy with envoy-bootstrap
    wasme deploy envoy webassemblyhub.io/rksmannem/add-header-cpp.wasm:v0.1.0 --envoy-image=istio/proxyv2:1.5.1 --bootstrap=envoy-bootstrap.yaml

 ----------------------------------------------------------------------







https://blog.red-badger.com/extending-istio-with-rust-and-webassembly


envoy with sandbox for wasme cpp filter:
https://blog.turbinelabs.io/getting-started-with-envoy-on-your-laptop-1b1a7073fd8e
https://www.bogotobogo.com/DevOps/Docker/Docker-Envoy-Getting-Started.php