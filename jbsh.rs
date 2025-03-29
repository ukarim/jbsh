#![allow(improper_ctypes)]

use std::io;
use std::io::Write;
use std::str::FromStr;

unsafe extern "C" {
    fn kill(pid: i32, sig: i32) -> i32;
    fn perror(msg: &[u8]);
}

const SIG_TERM: i32 = 9;

fn main() {
    write_to_std("Jason Bourne Shell 0.3.0\n\n", false);

    loop {
        write_to_std("bourne> ", false);
        let mut buffer = String::new();
        io::stdin().read_line(&mut buffer).expect("cannot read input from stdin");
        if buffer.is_empty() {
            continue;
        }
        let parts: Vec<&str> = buffer.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }
        match parts.get(0) {
            Some(&"exit") => break,
            Some(&"history") => write_to_std("I don't remember\n", false),
            Some(&"kill") => handle_kill(parts.get(1)),
            _ => write_to_std("Unknown command\n", true),
        }
    }
}

fn handle_kill(maybe_pid: Option<&&str>) {
    match maybe_pid {
        None => write_to_std("No pid argument provided\n", true),
        Some(pid) => {
            let pid_num: i32 = match FromStr::from_str(pid) {
                Ok(pid_num) => pid_num,
                Err(err) => {
                    write_to_std(&format!("Invalid pid provided: {}\n", err.to_string()), true);
                    return;
                }
            };
            unsafe {
                let sts = kill(pid_num, SIG_TERM);
                if sts != 0 {
                    perror("Cannot kill\0".as_bytes()); // do not forget to add null-terminator
                }
            }
        }
    }
}

fn write_to_std(msg: &str, err: bool) {
    if err {
        eprint!("{}", msg);
        io::stderr().flush().expect("error flushing stderr buffer");
    } else {
        print!("{}", msg);
        io::stdout().flush().expect("error flushing stdout buffer");
    }
}
