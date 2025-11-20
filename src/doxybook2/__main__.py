import argparse
import sys
from typing import List, Optional

from . import build_docs


def parse_args(argv: List[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Doxygen XML to Markdown via Doxybook2")
    parser.add_argument("-i", "--input", required=True, help="Path to the Doxygen XML directory")
    parser.add_argument("-o", "--output", required=True, help="Destination directory for generated files")
    parser.add_argument("-c", "--config", help="Optional JSON config file")
    parser.add_argument("-t", "--templates", help="Template directory override")
    parser.add_argument("--json", action="store_true", help="Generate JSON output only")
    parser.add_argument("-q", "--quiet", action="store_true", help="Suppress logging output")
    parser.add_argument("--debug-templates", action="store_true", help="Enable template debug JSON dumps")
    return parser.parse_args(argv)


def main(argv: Optional[List[str]] = None) -> int:
    args = parse_args(argv or sys.argv[1:])

    build_docs(
        input_dir=args.input,
        output_dir=args.output,
        config_path=args.config,
        templates_path=args.templates,
        json_only=args.json,
        quiet=args.quiet,
        debug_templates=args.debug_templates,
    )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())

