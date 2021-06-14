t # Simple Wayland Compositor

## 進捗

- クライアントからの通信を受けるソケットを立てただけ。
- `wl_compositor`, `xdg_wm_base`, `wl_shm` あたりのグローバルオブジェクトを実装。
  - とりあえず、clientが動くように実装してるので、全く実装していない機能とかも多い。

## build

```shell
meson build
ninja -C build
```

## Run

### environment

CLIモードとかじゃなくても大丈夫です。

### start server

```shell
WAYLAND_DEBUG=1 XDG_RUNTIME_DIR=~/.xdg ./build/server
```

### connect client

```shell
XDG_RUNTIME_DIR=~/.xdg weston-flower # or other wayland client
```

サーバー側のデバッグログからwayland クライアント ←→ サーバ間の通信ができていることが確認できます。

### Next to do

wl_outputの実装
