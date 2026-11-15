import csv
import random
from datetime import date, timedelta
from pathlib import Path

random.seed(42)
saida = Path("data/medicamentos.csv")
saida.parent.mkdir(exist_ok=True)

medicamentos = [
    ("Paracetamol 750 mg", "Paracetamol", "Analgesico"),
    ("Dipirona 500 mg", "Dipirona", "Analgesico"),
    ("Ibuprofeno 600 mg", "Ibuprofeno", "Anti-inflamatorio"),
    ("Amoxicilina 500 mg", "Amoxicilina", "Antibiotico"),
    ("Loratadina 10 mg", "Loratadina", "Antialergico"),
    ("Omeprazol 20 mg", "Omeprazol", "Gastroprotetor"),
    ("Metformina 850 mg", "Metformina", "Antidiabetico"),
    ("Losartana 50 mg", "Losartana", "Anti-hipertensivo"),
]
fabricantes = ["Fabricante A", "Fabricante B", "Fabricante C", "Fabricante D"]

hoje = date.today()
with saida.open("w", newline="", encoding="utf-8") as arq:
    writer = csv.writer(arq)
    writer.writerow(["codigo", "nome", "principio_ativo", "categoria",
                     "fabricante", "data_fabricacao", "data_validade",
                     "quantidade", "preco"])
    for i in range(1, 10001):
        nome, ativo, categoria = random.choice(medicamentos)
        # 15% vencidos; 20% vencem em até 30 dias; 15% em 31-90;
        # 50% vencem depois de 90 dias.
        r = random.random()
        if r < 0.15:
            dias_validade = -random.randint(1, 365)
        elif r < 0.35:
            dias_validade = random.randint(0, 30)
        elif r < 0.50:
            dias_validade = random.randint(31, 90)
        else:
            dias_validade = random.randint(91, 900)

        validade = hoje + timedelta(days=dias_validade)
        fabricacao = validade - timedelta(days=random.randint(180, 900))
        # Garantir que a fabricação não seja posterior à validade.
        if fabricacao > validade:
            fabricacao = validade - timedelta(days=30)

        writer.writerow([
            f"LOT{i:05d}", nome, ativo, categoria,
            random.choice(fabricantes),
            int(fabricacao.strftime("%Y%m%d")),
            int(validade.strftime("%Y%m%d")),
            random.randint(0, 1500),
            f"{random.uniform(1.5, 150):.2f}"
        ])

print(f"Base gerada: {saida} (10.000 registros)")
