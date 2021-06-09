# Simple Wayland Compositor

## 進捗

- クライアントからの通信を受けるソケットを立てただけ。

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
