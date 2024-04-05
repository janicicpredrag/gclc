const printLog = (log: string): number[] => {
  const terminal = document.getElementById("terminalPane")!;

  const escapedLog = log
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;")
    .replace(/^Error.*/gm, `<span class="error">$&</span>`)
    .replace(/^Warning.*/gm, `<span class="warning">$&</span>`);

  terminal.innerHTML = escapedLog;
  terminal.scrollTop = terminal.scrollHeight;

  const errorRegex = /^Error /m;
  const containsError = log.match(errorRegex);

  if (containsError) {
    const linNumbers = /^Error .*Line: (\d+).*/gm;
    const errors = [...log.matchAll(linNumbers)]
      .map((m) => (m.length > 1 ? m[1] : undefined))
      .filter((m) => m !== undefined)
      .map((m) => Number(m));

    if (errors.length > 0) {
      return errors;
    } else {
      return [-1];
    }
  }

  return [];
};

const resetTerminal = () => {
  const terminal = document.getElementById("terminalPane")!;
  terminal.innerHTML = "";
  terminal.scrollTop = terminal.scrollHeight;
};

export { printLog, resetTerminal };
