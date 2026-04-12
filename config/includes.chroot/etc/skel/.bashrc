# Check on your dawg when opening terminal
if [ -f "$HOME/.fetchos/dawg.json" ]; then
    /usr/local/bin/dawg-decay 2>/dev/null
fi
