#!/usr/bin/env bash
# Boots the image in QEMU, captures the UART log and checks milestone lines.
# A milestone reported as TODO is allowed. FAIL, a crash or a timeout is not.
# Mark a milestone as required by adding it to REQUIRED below once it is done.
set -euo pipefail

ELF="${1:-build/rv-bringup.elf}"
QEMU="${QEMU:-qemu-system-riscv64}"
LOG="$(mktemp)"

REQUIRED=(
  "[M0] boot + uart: OK"
  "[M1] traps: OK"
  # "[M2] timer interrupt: OK"
  # "[M3] pmp + s-mode: OK"
  # "[M4] plic + uart rx irq: OK"
  # "[M5] pcie enumeration: OK"
  # "[M6] pcie bar + mmio: OK"
  # "[M7] pcie dma + irq: OK"
)

set +e
echo "x" | timeout 10s "$QEMU" -machine virt -m 128M -smp 1 -nographic -bios none \
    -device edu -kernel "$ELF" -serial stdio -monitor none > "$LOG" 2>&1
rc=$?
set -e
cat "$LOG"

fail=0
[[ $rc -eq 124 ]] && { echo "ERROR: QEMU timed out (firmware hung?)"; fail=1; }
grep -q "^done" "$LOG" || { echo "ERROR: firmware did not reach 'done'"; fail=1; }
grep -q ": FAIL" "$LOG" && { echo "ERROR: a milestone self-test failed"; fail=1; }
for line in "${REQUIRED[@]}"; do
  grep -qF "$line" "$LOG" || { echo "ERROR: missing '$line'"; fail=1; }
done

[[ $fail -eq 0 ]] && echo "PASS" || echo "FAIL"
exit $fail
