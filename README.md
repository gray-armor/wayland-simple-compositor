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

surface の commit に呼応して
xdg_toplevelとsurfaceのconfigurationを行う。

- load_backend (main.c)
- load_headless_backend (main.c)
- wet_set_simple_head_configurator (main.c)
- simple_heads_changed (main.c)
- simple_head_enable (main.c)
- weston_output_enable (compositor.c)
- weston_compositor_add_output (compositor.c)
- weston_head_add_global (compositor.c) // create global output object

surface が commit されてからtoplevelがconfigureするまで
- surface_interface (compositor.c)
- surface_commit (compositor.c) l.3963
- weston_surface_commit (compositor.c)
- weston_surface_commit_state (compositor.c)
- wl_signal_emit(&surface->commit_signal, surface);
- weston_desktop_surface_create (surface.c) l.277 -> 276 // ここで desktop_surface にリスナーを追加している。
- weston_desktop_surface_surface_committed (surface.c) l.174
- weston_desktop_xdg_surface_internal_implementation (xdg-shell.c) .committed
- weston_desktop_xdg_surface_committed (xdg-shell.c)
- weston_desktop_xdg_toplevel_committed (xdg-shell.c)
- weston_desktop_xdg_toplevel_ensure_added (xdg-shell.c)
- weston_desktop_xdg_surface_schedule_configure (xdg-shell.c)
- weston_desktop_xdg_surface_send_configure (xdg-shell.c)
